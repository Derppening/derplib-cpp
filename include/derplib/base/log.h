#pragma once

#include <ctime>
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
  enum struct level {
    verbose,
    debug,
    info,
    warn,
    error
  };

  /**
   * \brief Constructor.
   *
   * Constructs a new logger.
   *
   * \param os destination of the logger
   * \param min_level minimum level of information that will be output to the logger
   */
  basic_logger(ostream& os, level min_level);

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
  void v(const string& message);

  /**
   * \brief Logs a debug message.
   * \param message message to log
   */
  void d(const string& message);

  /**
   * \brief Logs an information message.
   * \param message message to log
   */
  void i(const string& message);

  /**
   * \brief Logs a warning message.
   * \param message message to log
   */
  void w(const string& message);

  /**
   * \brief Logs an error message.
   * \param message message to log
   */
  void e(const string& message);

  /**
   * \brief Flushes the `ostream` associated to this logger.
   */
  void flush();

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

  /**
   * \brief Minimum level of messages that will be logged.
   */
  level _min_level;
  /**
   * \brief Reference to the output stream.
   */
  ostream& _ostream;
};

using logger = basic_logger<char>;
using wlogger = basic_logger<wchar_t>;

}  // namespace derplib

#include "derplib/base/log.ipp"
