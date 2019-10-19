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
  void daemon(std::size_t i);

  consumer_type _consumer = nullptr;

  std::atomic_bool _keep_alive;

  std::vector<std::deque<stdext::decay_t<InT>>> _buffers;
  std::vector<std::thread> _threads;
  std::vector<std::mutex> _mutexes;
  std::vector<std::condition_variable> _cvs;
};

template<typename InT, typename ConsumerT>
cfq_parallel_consumer<InT, ConsumerT>::cfq_parallel_consumer(const std::size_t concurrency,
                                                             const consumer_type& consumer) :
    _consumer(consumer),
    _keep_alive{true},
    _buffers{concurrency},
    _threads{concurrency},
    _mutexes{concurrency},
    _cvs{concurrency} {
  for (std::size_t i = 0; i < _threads.size(); ++i) {
    _threads[i] = std::thread(&cfq_parallel_consumer::daemon, this, i);
  }
}

template<typename InT, typename ConsumerT>
cfq_parallel_consumer<InT, ConsumerT>::cfq_parallel_consumer(const std::size_t concurrency, consumer_type&& consumer) :
    _consumer(consumer),
    _keep_alive{true},
    _buffers{concurrency},
    _threads{concurrency},
    _mutexes{concurrency},
    _cvs{concurrency} {
  for (std::size_t i = 0; i < _threads.size(); ++i) {
    _threads[i] = std::thread(&cfq_parallel_consumer::daemon, this, i);
  }
}

template<typename InT, typename ConsumerT>
cfq_parallel_consumer<InT, ConsumerT>::~cfq_parallel_consumer() {
  _keep_alive = false;

  for (auto& cv : _cvs) {
    cv.notify_all();
  }

  for (auto& thread : _threads) {
    if (thread.joinable()) {
      thread.join();
    }
  }
}

template<typename InT, typename ConsumerT>
void cfq_parallel_consumer<InT, ConsumerT>::push(const stdext::decay_t<InT>& value) {
  std::size_t n = std::numeric_limits<std::size_t>::max();
  std::size_t it = std::numeric_limits<std::size_t>::max();

  for (std::size_t i = 0; i < _buffers.size(); ++i) {
    std::size_t size = _buffers[i].size();
    if (n > size) {
      it = i;
      n = size;
    }
  }

  std::lock_guard<std::mutex> lk(_mutexes[it]);
  _buffers[it].push_back(value);
}

template<typename InT, typename ConsumerT>
void cfq_parallel_consumer<InT, ConsumerT>::push(stdext::decay_t<InT>&& value) {
  std::size_t it = std::numeric_limits<std::size_t>::max();

  for (std::size_t i = 0, n = std::numeric_limits<std::size_t>::max(); i < _buffers.size(); ++i) {
    std::size_t size = _buffers[i].size();
    if (n > size) {
      it = i;
      n = size;
    }
  }

  std::unique_lock<std::mutex> lk(_mutexes[it]);
  _buffers[it].push_back(value);

  lk.unlock();
  _cvs[it].notify_one();
}

template<typename InT, typename ConsumerT>
template<typename... Args>
void cfq_parallel_consumer<InT, ConsumerT>::emplace(Args&&... args) {
  std::size_t n = std::numeric_limits<std::size_t>::max();
  std::size_t it = std::numeric_limits<std::size_t>::max();

  for (std::size_t i = 0; i < _buffers.size(); ++i) {
    std::size_t size = _buffers[i].size();
    if (n > size) {
      it = i;
      n = size;
    }
  }

  std::lock_guard<std::mutex> lk(_mutexes[it]);
  _buffers[it].emplace_back(std::forward<Args>(args)...);
}

template<typename InT, typename ConsumerT>
void cfq_parallel_consumer<InT, ConsumerT>::daemon(const std::size_t i) {
  while (_keep_alive || !_buffers[i].empty()) {
    std::unique_lock<std::mutex> lk(_mutexes[i]);
    _cvs[i].wait(lk, [&] { return !_keep_alive || !_buffers[i].empty(); });

    if (!_keep_alive && _buffers[i].empty()) {
      break;
    }

    _consumer(_buffers[i].front());
    _buffers[i].pop_front();

    lk.unlock();
    _cvs[i].notify_one();
  }
}

}  // namespace container
}  // namespace derplib
