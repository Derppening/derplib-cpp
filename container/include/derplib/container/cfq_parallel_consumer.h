#pragma once

#include <atomic>
#include <condition_variable>
#include <limits>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include <derplib/stdext/type_traits.h>

namespace derplib {
inline namespace container {

/**
 * \brief A buffered consumer with parallel execution support.
 *
 * See \ref cfq_parallel_consumer<InT, ConsumerT>.
 *
 * \tparam InT The type of the elements to be processed.
 * \tparam ConsumerT The type of the functor to process the elements. Must have a prototype of
 * `void f(InT)`.
 */
template<typename InT, typename ConsumerT = void (*)(InT), typename = stdext::enable_if_invocable<ConsumerT>>
class cfq_parallel_consumer;

template<typename InT, typename ConsumerT>
class cfq_parallel_consumer<InT, ConsumerT> {
 public:
  /**
   * \brief Type of elements.
   */
  using value_type = InT;
  /**
   * \brief Type of functor.
   */
  using consumer_type = ConsumerT;

  /**
   * \param concurrency Number of concurrent consumers.
   * \param consumer Consumer function.
   */
  cfq_parallel_consumer(std::size_t concurrency, const consumer_type& consumer);

  /**
   * \param concurrency Number of concurrent consumers.
   * \param consumer Consumer function.
   */
  cfq_parallel_consumer(std::size_t concurrency, consumer_type&& consumer);

  ~cfq_parallel_consumer();

  /**
   * \brief Adds an element to the back.
   *
   * Appends the given element `value` to the back of the buffer. The new element is initialized as a copy of `value`.
   *
   * \param value The value of the element to append.
   */
  void push(const stdext::decay_t<InT>& value);

  /**
   * \brief Adds an element to the back.
   *
   * Appends the given element `value` to the back of the buffer. `value` is moved into the new element.
   *
   * \param value The value of the element to append.
   */
  void push(stdext::decay_t<InT>&& value);

  /**
   * \brief Constructs an element in-place at the back.
   *
   * Appends a new element to the back of the buffer.
   *
   * \tparam Args Argument types of the element constructor.
   * \param args Arguments to forward to the constructor of the element.
   */
  template<typename... Args>
  void emplace(Args&&... args);

 private:
  /**
   * \brief Daemon method for consuming the elements in the buffer.
   *
   * \param i The index of the thread.
   */
  void _daemon(std::size_t i);

  consumer_type _consumer_ = nullptr;

  std::atomic_bool _keep_alive_;

  std::vector<std::deque<stdext::decay_t<InT>>> _buffers_;
  std::vector<std::thread> _threads_;
  std::vector<std::mutex> _mutexes_;
  std::vector<std::condition_variable> _cvs_;
};

template<typename InT, typename ConsumerT>
cfq_parallel_consumer<InT, ConsumerT>::cfq_parallel_consumer(const std::size_t concurrency,
                                                             const consumer_type& consumer) :
    _consumer_(consumer),
    _keep_alive_{true},
    _buffers_{concurrency},
    _threads_{concurrency},
    _mutexes_{concurrency},
    _cvs_{concurrency} {
  for (std::size_t i{0}; i < _threads_.size(); ++i) {
    _threads_[i] = std::thread{&cfq_parallel_consumer::_daemon, this, i};
  }
}

template<typename InT, typename ConsumerT>
cfq_parallel_consumer<InT, ConsumerT>::cfq_parallel_consumer(const std::size_t concurrency, consumer_type&& consumer) :
    _consumer_(consumer),
    _keep_alive_{true},
    _buffers_{concurrency},
    _threads_{concurrency},
    _mutexes_{concurrency},
    _cvs_{concurrency} {
  for (std::size_t i{0}; i < _threads_.size(); ++i) {
    _threads_[i] = std::thread{&cfq_parallel_consumer::_daemon, this, i};
  }
}

template<typename InT, typename ConsumerT>
cfq_parallel_consumer<InT, ConsumerT>::~cfq_parallel_consumer() {
  _keep_alive_ = false;

  for (auto& cv : _cvs_) {
    cv.notify_all();
  }

  for (auto& thread : _threads_) {
    if (thread.joinable()) {
      thread.join();
    }
  }
}

template<typename InT, typename ConsumerT>
void cfq_parallel_consumer<InT, ConsumerT>::push(const stdext::decay_t<InT>& value) {
  std::size_t it{std::numeric_limits<std::size_t>::max()};

  for (std::size_t i{0}, n{std::numeric_limits<std::size_t>::max()}; i < _buffers_.size(); ++i) {
    std::size_t size{_buffers_[i].size()};
    if (n > size) {
      it = i;
      n = size;
    }
  }

  {
    std::lock_guard<std::mutex> lk{_mutexes_[it]};
    _buffers_[it].push_back(value);
  }

  _cvs_[it].notify_one();
}

template<typename InT, typename ConsumerT>
void cfq_parallel_consumer<InT, ConsumerT>::push(stdext::decay_t<InT>&& value) {
  std::size_t it{std::numeric_limits<std::size_t>::max()};

  for (std::size_t i{0}, n{std::numeric_limits<std::size_t>::max()}; i < _buffers_.size(); ++i) {
    std::size_t size{_buffers_[i].size()};
    if (n > size) {
      it = i;
      n = size;
    }
  }

  {
    std::unique_lock<std::mutex> lk{_mutexes_[it]};
    _buffers_[it].push_back(value);
  }

  _cvs_[it].notify_one();
}

template<typename InT, typename ConsumerT>
template<typename... Args>
void cfq_parallel_consumer<InT, ConsumerT>::emplace(Args&&... args) {
  std::size_t it{std::numeric_limits<std::size_t>::max()};

  for (std::size_t i{0}, n{std::numeric_limits<std::size_t>::max()}; i < _buffers_.size(); ++i) {
    std::size_t size{_buffers_[i].size()};
    if (n > size) {
      it = i;
      n = size;
    }
  }

  {
    std::lock_guard<std::mutex> lk{_mutexes_[it]};
    _buffers_[it].emplace_back(std::forward<Args>(args)...);
  }

  _cvs_[it].notify_one();
}

template<typename InT, typename ConsumerT>
void cfq_parallel_consumer<InT, ConsumerT>::_daemon(size_t i) {
  while (_keep_alive_ || !_buffers_[i].empty()) {
    {
      std::unique_lock<std::mutex> lk{_mutexes_[i]};
      _cvs_[i].wait(lk, [&] { return !_keep_alive_ || !_buffers_[i].empty(); });

      if (!_keep_alive_ && _buffers_[i].empty()) {
        break;
      }

      _consumer_(_buffers_[i].front());
      _buffers_[i].pop_front();
    }

    _cvs_[i].notify_one();
  }
}

}  // namespace container
}  // namespace derplib
