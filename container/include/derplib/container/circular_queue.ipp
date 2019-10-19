#include "derplib/stdext/version.h"

namespace derplib {
namespace container {

template<typename T, std::size_t N>
circular_queue<T, N>::circular_queue() noexcept : _data() {}

template<typename T, std::size_t N>
template<std::size_t SIZE>
circular_queue<T, N>::circular_queue(const std::array<T, SIZE>& cont)
noexcept(std::is_nothrow_copy_assignable<T>::value && std::is_nothrow_default_constructible<T>::value) :
    _begin(_data.begin()),
    _end(SIZE > N ? _data.begin() + N : _data.begin() + SIZE),
    _size(std::min(SIZE, N)) {
  std::copy(cont.begin(), cont.begin() + _size, _begin);
  std::fill(_end, _data.end(), T());
}

template<typename T, std::size_t N>
circular_queue<T, N>::circular_queue(container_type&& cont)
noexcept(std::is_nothrow_move_assignable<container_type>::value) :
    _data(std::move(cont)), _begin(_data.begin()), _end(_data.end()), _size(N) {}

template<typename T, std::size_t N>
circular_queue<T, N>::circular_queue(const circular_queue& other)
noexcept(std::is_nothrow_copy_constructible<container_type>::value) :
    _data(other._data),
    _begin(_data.begin() + (other._begin - other._data.begin())),
    _end(other._end == nullptr ? nullptr : _data.begin() + (other._end - other._data.begin())),
    _size(other._size) {}

template<typename T, std::size_t N>
circular_queue<T, N>::circular_queue(circular_queue&& other)
noexcept(std::is_nothrow_move_constructible<container_type>::value) :
    _data(std::move(other._data)),
    _begin(_data.begin() + (other._begin - other._data.begin())),
    _end(other._end == nullptr ? nullptr : _data.begin() + (other._end - other._data.begin())),
    _size(other._size) {}

template<typename T, std::size_t N>
circular_queue<T, N>& circular_queue<T, N>::operator=(const circular_queue& other)
noexcept(std::is_nothrow_copy_assignable<container_type>::value) {
  if (&other == this) {
    return *this;
  }

  _data = other._data;
  _begin = _data.begin() + (other._begin - other._data.begin());
  _end = other._end == nullptr ? nullptr : _data.begin() + (other._end - other._data.begin());
  _size = other._size;

  return *this;
}

template<typename T, std::size_t N>
circular_queue<T, N>& circular_queue<T, N>::operator=(circular_queue&& other)
noexcept(std::is_nothrow_move_assignable<container_type>::value) {
  if (&other == this) {
    return *this;
  }

  _data = std::move(other._data);
  _begin = _data.begin() + (other._begin - other._data.begin());
  _end = other._end == nullptr ? nullptr : _data.begin() + (other._end - other._data.begin());
  _size = other._size;

  return *this;
}

template<typename T, std::size_t N>
typename circular_queue<T, N>::reference circular_queue<T, N>::front() {
  if (empty()) {
    throw std::runtime_error("front(): no element");
  }

  return *_begin;
}

template<typename T, std::size_t N>
typename circular_queue<T, N>::const_reference circular_queue<T, N>::front() const {
  if (empty()) {
    throw std::runtime_error("front(): no element");
  }

  return *_begin;
}

template<typename T, std::size_t N>
typename circular_queue<T, N>::reference circular_queue<T, N>::back() {
  if (empty()) {
    throw std::runtime_error("back(): no element");
  }

  return _end[-1];
}

template<typename T, std::size_t N>
typename circular_queue<T, N>::const_reference circular_queue<T, N>::back() const {
  if (empty()) {
    throw std::runtime_error("front(): no element");
  }

  return _end[-1];
}

template<typename T, std::size_t N>
bool circular_queue<T, N>::empty() const noexcept { return _size == 0; }

template<typename T, std::size_t N>
typename circular_queue<T, N>::size_type circular_queue<T, N>::size() const noexcept { return _size; }

template<typename T, std::size_t N>
void circular_queue<T, N>::push(const value_type& value) {
  if (size() == N) {
    throw std::length_error("push(): max elements alloc'd");
  }

  if (empty() || _end == _data.end()) {
    _end = _data.begin();
  }

  *_end++ = value;
  ++_size;
}

template<typename T, std::size_t N>
void circular_queue<T, N>::push(value_type&& value) {
  if (size() == N) {
    throw std::length_error("push(): max elements alloc'd");
  }

  if (empty() || _end == _data.end()) {
    _end = _data.begin();
  }

  *_end++ = std::move(value);
  ++_size;
}

#if defined(DERPLIB_HAS_CPP17_SUPPORT)
  template<typename T, std::size_t N>
  template<typename... Args>
  decltype(auto) circular_queue<T, N>::emplace(Args&& ... args) {
    if (size() == N) {
      throw std::length_error("push(): max elements alloc'd");
    }

    if (empty() || _end == _data.end()) {
      _end = _data.begin();
    }

    *_end++ = T(std::forward<Args>(args)...);
    ++_size;

    return back();
  }
#else
template<typename T, std::size_t N>
template<typename... Args>
void circular_queue<T, N>::emplace(Args&& ... args) {
  if (size() == N) {
    throw std::length_error("push(): max elements alloc'd");
  }

  if (empty() || _end == _data.end()) {
    _end = _data.begin();
  }

  *_end++ = T(std::forward<Args>(args)...);
  ++_size;
}
#endif  // defined(DERPLIB_HAS_CPP17_SUPPORT)

template<typename T, std::size_t N>
void circular_queue<T, N>::pop() noexcept(std::is_nothrow_default_constructible<T>::value) {
  if (empty()) {
    return;
  }

  *_begin = T();
  ++_begin;
  --_size;

  if (_size == 0) {
    _begin = _data.begin();
    _end = nullptr;
  }
}

template<typename T, std::size_t N>
void circular_queue<T, N>::swap(circular_queue& other) noexcept(stdext::is_nothrow_swappable<container_type>::value) {
  std::swap(this->_data, other._data);
  std::swap(this->_begin, other._begin);
  std::swap(this->_end, other._end);
  std::swap(this->_size, other._size);
}

template<typename T, std::size_t N>
void swap(circular_queue<T, N>& lhs, circular_queue<T, N>& rhs)
noexcept(stdext::is_nothrow_swappable<typename circular_queue<T, N>::container_type>::value) {
  lhs.swap(rhs);
}

}  // namespace container
}  // namespace derplib
