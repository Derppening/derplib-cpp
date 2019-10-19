#pragma once

#include <atomic>
#include <chrono>
#include <functional>
#include <iostream>
#include <memory>
#include <thread>

#include <derplib/stdext/type_traits.h>

namespace derplib {

/**
 * \brief A simple timer that invokes a callback after a fixed amount of time.
 *
 * \tparam Func function type. Must be `void f()`.
 */
template<typename Func = std::function<void()>, typename = stdext::enable_if_invocable<Func>>
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
  timer(std::chrono::duration<Rep, Period> duration, const Func& callback) :
      timer(duration, std::move(callback), duration) {}

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
  timer(std::chrono::duration<Rep, Period> duration,
        const Func& callback,
        std::chrono::duration<Rep, Period> poll_rate);

  timer(const timer& other) : timer(other._duration, other._callback, other._poll_rate) {}
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

  /**
   * \brief Starts the timer.
   */
  void start();
  /**
   * \brief Stops the timer.
   */
  void stop();
  /**
   * \return Whether the timer is active.
   */
  bool active() const { return _state == state::active; }
  /**
   * \return Whether the timer has expired.
   */
  bool expired() const { return _state == state::expired; }

 private:
  enum struct state { halted = -1, not_started, active, expired };

  /**
   * \brief Daemon function for polling the timer.
   */
  void countdown_daemon();

  std::atomic<state> _state;
  std::atomic<bool> _keep_alive;

  const std::chrono::nanoseconds _duration;
  const std::chrono::nanoseconds _poll_rate;

  Func _callback;
  std::thread _thread;

  std::chrono::time_point<std::chrono::steady_clock> _end_time = {};
};

template<typename Func>
template<typename Rep, typename Period>
timer<Func>::timer(std::chrono::duration<Rep, Period> duration,
                   const Func& callback,
                   std::chrono::duration<Rep, Period> poll_rate) :
    _state(state::not_started),
    _keep_alive(true),
    _duration(std::chrono::nanoseconds(duration)),
    _poll_rate(std::chrono::nanoseconds(poll_rate)),
    _callback(callback) {}

template<typename Func>
timer<Func>& timer<Func>::operator=(const timer& other) {
  if (this != &other) {
    _state = state::not_started;
    _keep_alive = true;
    _duration = other._duration;
    _poll_rate = other._poll_rate;
    _callback = other._callback;
  }

  return *this;
}

template<typename Func>
timer<Func>::~timer() {
  _keep_alive = false;

  if (_thread.joinable()) {
    _thread.join();
  }
}

template<typename Func>
void timer<Func>::start() {
  if (_state == state::active) {
    throw std::logic_error("Attempted to start a running timer");
  } else if (_state == state::expired) {
    throw std::logic_error("Attempted to restart an expired timer");
  }

  _state = state::active;
  _end_time = std::chrono::steady_clock::now() + _duration;
  _thread = std::thread(&timer::countdown_daemon, this);
}

template<typename Func>
void timer<Func>::stop() {
  if (_state == state::not_started) {
    return;
  }
  if (_state != state::active) {
    throw std::logic_error("Attempted to stop a halted timer");
  }

  _keep_alive = false;

  if (_thread.joinable()) {
    _thread.join();
  }

  _thread = std::thread();
}

template<typename Func>
void timer<Func>::countdown_daemon() {
  _state = state::active;

  while (std::chrono::steady_clock::now() < _end_time) {
    if (!_keep_alive) {
      _state = state::halted;

      return;
    }

    std::this_thread::sleep_for(_poll_rate);
  }

  _state = state::expired;
  if (_callback) {
    _callback();
  }
}

}  // namespace derplib
