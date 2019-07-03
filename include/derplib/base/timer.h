// Implements a basic timer.

#pragma once

#include <atomic>
#include <chrono>
#include <functional>
#include <iostream>
#include <memory>
#include <thread>

#include "derplib/internal/common_type_traits.h"

namespace derplib {

/**
 * @brief A simple timer that invokes a callback after a fixed amount of time.
 *
 * @tparam Func function type. Must be `void f()`.
 */
template<typename Func = std::function<void()>, typename = internal::enable_if_invocable<Func>>
class timer;

template<typename Func>
class timer<Func> {
 public:
  /**
   * \brief Constructs an instance of `timer`.
   *
   * \tparam Rep an arithmetic type representing the number of ticks
   * \tparam Period a `std::ratio` representing the tick period
   * \param duration duration of the timer
   * \param callback callback function to run when the timer expires
   */
  template<typename Rep, typename Period>
  timer(std::chrono::duration<Rep, Period> duration, const Func& callback);

  /**
   * \brief Constructs an instance of `timer`.
   *
   * \tparam Rep an arithmetic type representing the number of ticks
   * \tparam Period a `std::ratio` representing the tick period
   * \param duration duration of the timer
   * \param callback callback function to run when the timer expires
   * \param poll_rate rate at which the timer checks whether it should terminate
   */
  template<typename Rep, typename Period>
  timer(std::chrono::duration<Rep, Period> duration, const Func& callback, std::chrono::duration<Rep, Period> poll_rate);

  timer(const timer& other);
  timer(timer&& other) noexcept = default;

  /**
   * \brief Copy-assignment operator.
   *
   * Copies `duration`, `callback` and `poll_rate` from the `other` timer, but does not start it.
   *
   * \param other `timer` to copy from.
   * \return `*this`.
   */
  timer& operator=(const timer& other);
  timer& operator=(timer&& other) noexcept = default;

  ~timer();

  void start();
  void stop();
  bool active() const;
  bool expired() const;

 private:
  enum struct state {
    halted = -1,
    not_started,
    active,
    expired
  };

  void countdown_daemon();

  std::atomic<state> _state;
  std::atomic<bool> _keep_alive;

  const std::chrono::nanoseconds _duration;
  const std::chrono::nanoseconds _poll_rate;

  Func _callback;
  std::thread _thread;

  std::chrono::time_point<std::chrono::steady_clock> _end_time = {};
};
}  // namespace derplib

#include "derplib/base/timer.ipp"
