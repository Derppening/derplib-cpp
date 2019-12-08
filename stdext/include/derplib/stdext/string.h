#pragma once

#include <iterator>
#include <sstream>
#include <string>
#include <vector>

namespace derplib {
inline namespace stdext {

/**
 * \brief Splits a string by a given delimiter.
 *
 * \tparam CharT character type
 * \param input input string
 * \param delimiter delimiter string
 * \param limit maximum number of substrings to return. 0 implies no limit
 * \return `std::vector` of substrings
 */
template<typename CharT>
std::vector<std::basic_string<CharT>>
split_string(const std::basic_string<CharT>& input, const std::basic_string<CharT>& delimiter, unsigned limit = 0);

/**
 * \brief Splits a string by a given delimiter.
 *
 * \tparam CharT character type
 * \param input input string
 * \param delimiter delimiter character
 * \param limit maximum number of substrings to return. 0 implies no limit
 * \return `std::vector` of substrings
 */
template<typename CharT>
std::vector<std::basic_string<CharT>>
split_string(const std::basic_string<CharT>& input, CharT delimiter, unsigned limit = 0);

/**
 * \brief Splits a string by a given delimiter.
 *
 * \tparam CharT character type
 * \param begin iterator to the first character to consider
 * \param end iterator to the past-the-end character to consider
 * \param delimiter delimiter character
 * \param limit maximum number of substrings to return. 0 implies no limit
 * \return `std::vector` of substrings.
 */
template<typename CharT>
std::vector<std::basic_string<CharT>> split_string(typename std::basic_string<CharT>::const_iterator begin,
                                                   typename std::basic_string<CharT>::const_iterator end,
                                                   CharT delimiter,
                                                   unsigned limit = 0);

/**
 * \brief Joins a string from a `std::vector` using a given separator.
 *
 * \tparam T `std::basic_string`, or any character sequence container which implements `value_type`
 * typedef and `std::ostream& operator<<(std::ostream&, T)`.
 * \param begin iterator to the first element to join
 * \param end iterator to the past-the-last element to join
 * \param separator separator between two string elements
 * \return A string with all elements joint together.
 */
template<typename T>
T join_to_string(typename std::vector<T>::const_iterator begin,
                 typename std::vector<T>::const_iterator end,
                 const T& separator);

template<typename T>
T join_to_string(typename std::vector<T>::const_iterator begin,
                 typename std::vector<T>::const_iterator end,
                 typename T::value_type separator) {
  return join_to_string(begin, end, T{1, separator});
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

/**
 * \brief Converts the given string to upper case.
 *
 * \param[in,out] str string to process
 * \return Reference to `str`.
 */
std::string& toupper(std::string& str);
/**
 * \brief Converts the given string to lower case.
 *
 * \param[in,out] str string to process
 * \return Reference to `str`.
 */
std::string& tolower(std::string& str);

template<typename CharT>
std::vector<std::basic_string<CharT>>
split_string(const std::basic_string<CharT>& input, const std::basic_string<CharT>& delimiter, unsigned limit) {
  using string_type = typename std::basic_string<CharT>;
  std::vector<string_type> vs{};

  std::size_t pos{0};
  std::size_t end{};
  while ((end = input.find(delimiter, pos)) != string_type::npos) {
    vs.emplace_back(input.substr(pos, end - pos));
    pos = end + delimiter.length();

    if (limit != 0 && vs.size() >= limit) {
      return vs;
    }
  }

  if (pos < input.size()) {
    vs.emplace_back(input.substr(pos));
  }

  return vs;
}

template<typename CharT>
std::vector<std::basic_string<CharT>>
split_string(const std::basic_string<CharT>& input, const CharT delimiter, unsigned limit) {
  using string_type = typename std::basic_string<CharT>;
  std::vector<string_type> vs{};

  std::size_t pos{0};
  std::size_t end{};
  while ((end = input.find(delimiter, pos)) != string_type::npos) {
    vs.emplace_back(input.substr(pos, end - pos));
    pos = end + 1;

    if (limit != 0 && vs.size() >= limit) {
      return vs;
    }
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
  std::vector<string_type> vs{};

  std::string buffer{};
  for (auto it = begin; it != end; ++it) {
    if (*it == delimiter) {
      vs.emplace_back(std::move(buffer));
      buffer = std::string{};

      if (limit != 0 && vs.size() >= limit) {
        return vs;
      }
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
  std::basic_stringstream<typename T::value_type> arg{};
  std::copy(begin, end, std::ostream_iterator<T>(arg, separator.c_str()));
  auto str{arg.str()};
  str.erase(str.length() - separator.length());
  return str;
}

}  // namespace stdext
}  // namespace derplib
