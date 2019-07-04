#pragma once

#include <chrono>

namespace derplib {
/**
 * \brief A basic stopwatch, utilizing `std::chrono::steady_clock`.
 */
class stopwatch {
 public:
  stopwatch() = default;

  stopwatch(const stopwatch& other) = delete;
  stopwatch(stopwatch&& other) noexcept = default;

  stopwatch& operator=(const stopwatch& other) = delete;
  stopwatch& operator=(stopwatch&& other) noexcept = default;

  /**
   * \brief Starts the stopwatch.
   */
  void start();
  /**
   * \brief Stops the stopwatch.
   */
  void stop();

  /**
   * \brief Retrieves either the time elapsed since the stopwatch started, or the duration of the
   * stopwatch if the stopwatch is stopped.
   *
   * \tparam ToDuration Target type of `std::chrono::duration`.
   * \return Time elapsed in terms of `ToDuration`, in numeric form.
   */
  template<typename ToDuration = std::chrono::nanoseconds>
  typename ToDuration::rep time() const { return time_raw<ToDuration>().count(); }
  /**
   * \brief Retrieves either the time elapsed since the stopwatch started, or the duration of the
   * stopwatch if the stopwatch is stopped.
   *
   * \tparam ToDuration Target type of `std::chrono::duration`.
   * \return Time elapsed in terms of `ToDuration`.
   */
  template<typename ToDuration = std::chrono::nanoseconds>
  ToDuration time_raw() const {
    return std::chrono::duration_cast<ToDuration>((_is_active ? std::chrono::steady_clock::now() : _end ) - _start);
  }

 private:
  std::chrono::time_point<std::chrono::steady_clock> _start;
  std::chrono::time_point<std::chrono::steady_clock> _end;

  bool _is_active = false;
};
}  // namespace derplib
