#pragma once

#include <iterator>
#include <sstream>
#include <string>
#include <vector>

namespace derplib {
namespace util {

// TODO(Derppening): Move to stdext

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
std::vector<std::basic_string<CharT>> split_string(const std::basic_string<CharT>& input,
                                                   const std::basic_string<CharT>& delimiter,
                                                   unsigned limit = 0);

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
std::vector<std::basic_string<CharT>> split_string(const std::basic_string<CharT>& input,
                                                   CharT delimiter,
                                                   unsigned limit = 0);

// TODO(Derppening): Add overloads for split_string(iterator, iterator, delimiter, limit)

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
T join_to_string(typename std::vector<T>::const_iterator begin, typename std::vector<T>::const_iterator end, const T& separator);

template<typename T>
T join_to_string(typename std::vector<T>::const_iterator begin,
                 typename std::vector<T>::const_iterator end,
                 typename T::value_type separator);

template<typename T>
std::basic_string<T> join_to_string(const std::vector<std::basic_string<T>>& input,
                                    const std::basic_string<T>& separator);

template<typename T>
std::basic_string<T> join_to_string(const std::vector<std::basic_string<T>>& input, T separator);

std::string& toupper(std::string& str);
std::string& tolower(std::string& str);

}  // namespace util
}  // namespace derplib

#include "derplib/util/string.ipp"
