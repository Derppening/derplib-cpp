#include <chrono>
#include <iostream>

namespace derplib {

template<typename CharT>
std::unique_ptr<basic_logger<CharT>> basic_logger<CharT>::instance;

template<typename CharT>
basic_logger<CharT>::basic_logger(basic_logger::ostream& os, basic_logger::level min_level) :
    _min_level(min_level), _ostream(os) {}

template<typename CharT>
void basic_logger<CharT>::v(const basic_logger::string& message) {
  print_helper(message, level::verbose);
}

template<typename CharT>
void basic_logger<CharT>::d(const basic_logger::string& message) {
  print_helper(message, level::debug);
}

template<typename CharT>
void basic_logger<CharT>::i(const basic_logger::string& message) {
  print_helper(message, level::info);
}

template<typename CharT>
void basic_logger<CharT>::w(const basic_logger::string& message) {
  print_helper(message, level::warn);
}

template<typename CharT>
void basic_logger<CharT>::e(const basic_logger::string& message) {
  print_helper(message, level::error);
}

template<typename CharT>
void basic_logger<CharT>::flush() { _ostream.flush(); }

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

}  // namespace derplib
