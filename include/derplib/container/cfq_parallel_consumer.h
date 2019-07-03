#pragma once

#include <atomic>
#include <condition_variable>
#include <limits>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace derplib {
namespace container {

#if defined(__cpp_lib_is_invocable) || __cplusplus > 201402L
template<typename Fn, typename... ArgTypes>
using enable_if_invocable = typename std::enable_if<std::is_invocable<Fn, ArgTypes...>::value>;
#else
template<typename Fn, typename... ArgTypes>
using enable_if_invocable = std::true_type;
#endif  // defined(__cpp_lib_is_invocable) || __cplusplus > 201402L

#if __cplusplus > 201103L
template<typename T>
using decay_t = std::decay_t<T>;
#else
template<typename T>
using decay_t = typename std::decay<T>::type;
#endif  // __cplusplus > 201103L

/**
 * \brief A buffered consumer with parallel execution support.
 *
 * See \ref cfq_parallel_consumer<InT, ConsumerT>.
 *
 * \tparam InT The type of the elements to be processed.
 * \tparam ConsumerT The type of the functor to process the elements. Must have a prototype of
 * `void f(InT)`.
 */
template<typename InT, typename ConsumerT = void (*)(InT), typename = enable_if_invocable<ConsumerT>>
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
  void push(const decay_t<InT>& value);

  /**
   * \brief Adds an element to the back.
   *
   * Appends the given element `value` to the back of the buffer. `value` is moved into the new element.
   *
   * \param value The value of the element to append.
   */
  void push(decay_t<InT>&& value);

  /**
   * \brief Constructs an element in-place at the back.
   *
   * Appends a new element to the back of the buffer.
   *
   * \tparam Args Argument types of the element constructor.
   * \param args Arguments to forward to the constructor of the element.
   */
  template<typename... Args>
  void emplace(Args&& ... args);

 private:
  /**
   * \brief Daemon method for consuming the elements in the buffer.
   *
   * \param i The index of the thread.
   */
  void daemon(std::size_t i);

  consumer_type _consumer = nullptr;

  std::atomic_bool _keep_alive;

  std::vector<std::deque<decay_t<InT>>> _buffers;
  std::vector<std::thread> _threads;
  std::vector<std::mutex> _mutexes;
  std::vector<std::condition_variable> _cvs;
};

}  // namespace container
}  // namespace derplib

#include "derplib/container/cfq_parallel_consumer.ipp"
