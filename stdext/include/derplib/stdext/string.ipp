#include <string>
#include <vector>

namespace derplib {
namespace util {

template<typename CharT>
std::vector<std::basic_string<CharT>> split_string(const std::basic_string<CharT>& input,
                                                   const std::basic_string<CharT>& delimiter,
                                                   unsigned limit) {
  using string_type = typename std::basic_string<CharT>;
  std::vector<string_type> vs;

  std::size_t pos = 0;
  std::size_t end;
  while ((end = input.find(delimiter, pos)) != string_type::npos) {
    vs.emplace_back(input.substr(pos, end - pos));
    pos = end + delimiter.length();

    if (limit != 0 && vs.size() >= limit) { return vs; }
  }

  if (pos < input.size()) {
    vs.emplace_back(input.substr(pos));
  }

  return vs;
}

template<typename CharT>
std::vector<std::basic_string<CharT>> split_string(const std::basic_string<CharT>& input,
                                                   const CharT delimiter,
                                                   unsigned limit) {
  using string_type = typename std::basic_string<CharT>;
  std::vector<string_type> vs;

  std::size_t pos = 0;
  std::size_t end;
  while ((end = input.find(delimiter, pos)) != string_type::npos) {
    vs.emplace_back(input.substr(pos, end - pos));
    pos = end + 1;

    if (limit != 0 && vs.size() >= limit) { return vs; }
  }

  if (pos < input.size()) {
    vs.emplace_back(input.substr(pos));
  }

  return vs;
}

template<typename CharT>
std::vector<std::basic_string<CharT>> split_string(typename std::basic_string<CharT>::const_iterator begin,
                                                   typename std::basic_string<CharT>::const_iterator end,
                                                   CharT delimiter,
                                                   unsigned limit) {
  using string_type = typename std::basic_string<CharT>;
  std::vector<string_type> vs;

  std::string buffer;
  for (auto it = begin; it != end; ++it) {
    if (*it == delimiter) {
      vs.emplace_back(std::move(buffer));
      buffer = std::string();

      if (limit != 0 && vs.size() >= limit) { return vs; }
    } else {
      buffer.append(*it);
    }
  }

  if (!buffer.empty()) {
    vs.emplace_back(std::move(buffer));
  }

  return vs;
}

template<typename T>
T join_to_string(typename std::vector<T>::const_iterator begin,
                 typename std::vector<T>::const_iterator end,
                 const T& separator) {
  std::basic_stringstream<typename T::value_type> arg;
  std::copy(begin, end, std::ostream_iterator<T>(arg, separator.c_str()));
  auto str = arg.str();
  str.erase(str.length() - separator.length());
  return str;
}

template<typename T>
T join_to_string(typename std::vector<T>::const_iterator begin,
                 typename std::vector<T>::const_iterator end,
                 typename T::value_type separator) {
  auto sseparator = T(1, separator);
  return join_to_string(begin, end, sseparator);
}

template<typename T>
std::basic_string<T> join_to_string(const std::vector<std::basic_string<T>>& input,
                                    const std::basic_string<T>& separator) {
  return join_to_string<std::basic_string<T>>(input.begin(), input.end(), separator);
}

template<typename T>
std::basic_string<T> join_to_string(const std::vector<std::basic_string<T>>& input, T separator) {
  return join_to_string<std::basic_string<T>>(input.begin(), input.end(), separator);
}

}  // namespace util
}  // namespace derplib
