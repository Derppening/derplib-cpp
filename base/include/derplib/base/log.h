#pragma once

#include <ctime>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include <derplib/stdext/memory.h>

namespace derplib {

/**
 * \brief Simple logger for logging information.
 *
 * \tparam CharT character type
 */
template<typename CharT>
class basic_logger {
 private:
  using ostringstream = std::basic_ostringstream<CharT>;

 public:
  using char_type = CharT;
  using ostream = std::basic_ostream<char_type>;
  using string = std::basic_string<char_type>;

  /**
   * \brief Reports errors that are caused by incorrect initialization of the logger, and subsequently leading to the
   * logger to be in an invalid state.
   */
  class invalid_state final : public std::logic_error {
   public:
    explicit invalid_state(const char* what_arg) : logic_error(what_arg) {}

    explicit invalid_state(const std::string& what_arg) : logic_error(what_arg) {}
  };

  /**
   * \brief Level of logging.
   */
  enum struct level { verbose, debug, info, warn, error };

  /**
   * \brief Constructor.
   *
   * Constructs a new logger.
   *
   * \param os destination of the logger
   * \param min_level minimum level of information that will be output to the logger
   */
  basic_logger(ostream& os, level min_level) : _min_level(min_level), _ostream(os) {}

  /**
   * \brief Copy constructor.
   *
   * Deleted to avoid unintentional copies of a logger.
   */
  basic_logger(const basic_logger&) = delete;
  basic_logger(basic_logger&&) noexcept = default;

  /**
   * \brief Copy assignment operator.
   *
   * Deleted to avoid unintentional copies of a logger.
   */
  basic_logger& operator=(const basic_logger&) = delete;
  basic_logger& operator=(basic_logger&&) noexcept = default;

  /**
   * \brief Logs a verbose message.
   * \param message message to log
   */
  void v(const string& message) { print_helper(message, level::verbose); }

  /**
   * \brief Logs a debug message.
   * \param message message to log
   */
  void d(const string& message) { print_helper(message, level::debug); }

  /**
   * \brief Logs an information message.
   * \param message message to log
   */
  void i(const string& message) { print_helper(message, level::info); }

  /**
   * \brief Logs a warning message.
   * \param message message to log
   */
  void w(const string& message) { print_helper(message, level::warn); }

  /**
   * \brief Logs an error message.
   * \param message message to log
   */
  void e(const string& message) { print_helper(message, level::error); }

  /**
   * \brief Flushes the `ostream` associated to this logger.
   */
  void flush() { _ostream.flush(); }

  /**
   * \brief Creates a default logger.
   *
   * When a default logger is created, this instance can be accessed by `logger::get_default()` to avoid passing or
   * creating loggers while calling functions.
   *
   * \param os destination of the logger
   * \param min_level minimum level of information that will be output to the logger
   * \return The default logger instance.
   *
   * \throws `invalid_state` if a default logger has already been created.
   */
  static basic_logger<CharT>& make_default(ostream& os, level min_level = level::verbose);

  /**
   * \brief Replaces the default logger.
   *
   * Flushes the current default logger if it exists, and replaces the current instance with a new one created by the
   * specified parameters.
   *
   * \param os destination of the new logger
   * \param min_level minimum level of information that will be output to the new logger
   * \return The new default logger instance.
   */
  static basic_logger<CharT>& replace_default(ostream& os, level min_level = level::verbose);

  /**
   * \brief Returns the default logger.
   *
   * \return The default logger.
   * \throws `invalid_state` if a default logger has not been created yet.
   */
  static basic_logger<CharT>& get_default();

 private:
  static std::unique_ptr<basic_logger<CharT>> instance;

  /**
   * \return String representing the current time.
   */
  static std::basic_string<CharT> fmt_time();

  /**
   * \brief Helper function for formatting and outputting messages.
   *
   * \param message message to output
   * \param p_level level of the message
   */
  void print_helper(const string& message, level p_level);

  level _min_level;
  ostream& _ostream;
};

template<typename CharT>
std::unique_ptr<basic_logger<CharT>> basic_logger<CharT>::instance;

template<typename CharT>
basic_logger<CharT>& basic_logger<CharT>::make_default(basic_logger::ostream& os, basic_logger::level min_level) {
  if (instance != nullptr) {
    throw basic_logger::invalid_state("Default logger already initialized");
  }

  instance = stdext::make_unique<basic_logger<CharT>>(os, min_level);
  return get_default();
}

template<typename CharT>
basic_logger<CharT>& basic_logger<CharT>::replace_default(basic_logger::ostream& os, basic_logger::level min_level) {
  if (instance != nullptr) {
    instance->flush();
  }
  instance = stdext::make_unique<basic_logger<CharT>>(os, min_level);

  return get_default();
}

template<typename CharT>
basic_logger<CharT>& basic_logger<CharT>::get_default() {
  if (instance == nullptr) {
    throw basic_logger::invalid_state("Default logger not initialized");
  }

  return *instance;
}

template<typename CharT>
std::basic_string<CharT> basic_logger<CharT>::fmt_time() {
  // https://stackoverflow.com/questions/24686846/get-current-time-in-milliseconds-or-hhmmssmmm-format

  auto now = std::chrono::system_clock::now();
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

  auto time = std::chrono::system_clock::to_time_t(now);
  std::tm tm = *std::localtime(&time);

  basic_logger::ostringstream ss;
  ss << std::put_time(&tm, "%m-%d %H:%M:%S") << '.' << std::setfill('0') << std::setw(3) << ms.count();
  return ss.str();
}

template<typename CharT>
void basic_logger<CharT>::print_helper(const basic_logger::string& message, basic_logger::level p_level) {
  if (int(p_level) < int(_min_level)) {
    return;
  }

  _ostream << fmt_time() << '\t';

  switch (p_level) {
    case level::verbose:
      _ostream << 'V';
      break;
    case level::debug:
      _ostream << 'D';
      break;
    case level::info:
      _ostream << 'I';
      break;
    case level::warn:
      _ostream << 'W';
      break;
    case level::error:
      _ostream << 'E';
      break;
  }

  _ostream << ": " << message << '\n';
}

using logger = basic_logger<char>;
using wlogger = basic_logger<wchar_t>;

}  // namespace derplib
