#pragma once

#include <algorithm>
#include <array>
#include <utility>

#include <derplib/stdext/type_traits.h>
#include <derplib/stdext/version.h>

namespace derplib {
inline namespace container {

#include <derplib/internal/common_macros_begin.h>

/**
 * A queue implementation using an \c std::array, implemented in a circular manner.
 *
 * \tparam T Type of the stored elements.
 * \tparam N Maximum elements that can be stored.
 */
template<typename T, std::size_t N>
class circular_queue {
 public:
  static_assert(N > 0, "Circular Queue must have non-zero capacity");

  /**
   * \brief Type of the underlying container.
   */
  using container_type = std::array<T, N>;
  /**
   * \brief Type of the stored elements. Equivalent to `T`.
   */
  using value_type = typename container_type::value_type;
  /**
   * \brief Type used to represent sizes and indices.
   */
  using size_type = typename container_type::size_type;
  /**
   * \brief Reference type for the stored elements. Equivalent to `T&`.
   */
  using reference = typename container_type::reference;
  /**
   * \brief Constant reference type for the stored elements. Equivalent to `const T&`.
   */
  using const_reference = typename container_type::const_reference;

  /**
   * Default constructor. Value-initializes the container.
   */
  circular_queue() noexcept : _data_() {}

  /**
   * \brief Conversion constructor from \c std::array.
   *
   * Converts an \c std::array to a circular_queue. If the buffer of this object is smaller than the array, the first
   * \c N elements will be copied. Otherwise, the elements from the array will be copied, and the remaining elements
   * will be default-constructed.
   *
   * The \c size() after initialization will be the equivalent of `std::min(SIZE, N)`.
   *
   * \tparam Size Size of the \c std::array.
   * \param[in] cont Data to copy from.
   */
  template<std::size_t Size>
  explicit circular_queue(const std::array<T, Size>& cont) noexcept(
      std::is_nothrow_copy_assignable<T>::value&& std::is_nothrow_default_constructible<T>::value);

  /**
   * \brief Move conversion constructor from \c std::array.
   *
   * Move-constructs the underlying \c std::array with `std::move(cont)`.
   *
   * The \c size() after initialization will be `N`.
   *
   * \param[in] cont Container to initialize from.
   */
  explicit circular_queue(container_type&& cont) noexcept(std::is_nothrow_move_assignable<container_type>::value);

  /**
   * \brief Copy constructor.
   *
   * Copy-constructs this circular_queue with another one.
   *
   * \param[in] other The source circular queue to copy from.
   */
  circular_queue(const circular_queue& other) noexcept(std::is_nothrow_copy_constructible<container_type>::value);

  /**
   * \brief Move constructor.
   *
   * Move-constructs this circular queue with another one.
   *
   * \param[in] other The source circular queue to move from.
   */
  circular_queue(circular_queue&& other) noexcept(std::is_nothrow_move_constructible<container_type>::value);

  /**
   * \brief Default destructor.
   */
  ~circular_queue() = default;

  /**
   * \brief Copy assignment operator.
   *
   * Replaces the contents with a copy of \p other.
   *
   * \param other The circular_queue to copy from.
   * \return `*this`.
   */
  circular_queue&
  operator=(const circular_queue& other) & noexcept(std::is_nothrow_copy_assignable<container_type>::value);

  /**
   * \brief Move assignment operator.
   *
   * Replaces the contents with `other` using move semantics.
   *
   * \param other The circular_queue to move from.
   * \return `*this`.
   */
  circular_queue& operator=(circular_queue&& other) & noexcept(std::is_nothrow_move_assignable<container_type>::value);

  /**
   * \brief Returns a reference to the first element.
   *
   * The behavior is undefined when there is no element in the circular_queue, and `NOEXCEPT` is true.
   *
   * \return Reference to the first element.
   * \throw std::runtime_error when there is no element in the circular_queue.
   */
  reference front();

  /**
   * \brief Returns a constant reference to the first element.
   *
   * If there is no element in the circular_queue and `NOEXCEPT` is true, a default-constructed `T` will be returned.
   *
   * \return Constant reference to the first element.
   * \throw std::runtime_error when there is no element in the circular_queue.
   */
  const_reference front() const;

  /**
   * \brief Returns a reference to the last element.
   *
   * The behavior is undefined if there is no element in the circular_queue and `NOEXCEPT` is true.
   *
   * \return Reference to the last element.
   * \throw std::runtime_error when there is no element in the circular queue.
   */
  reference back();

  /**
   * \brief Returns a constant reference to the last element.
   *
   * If there is no element in the circular_queue and `NOEXCEPT` is true, a default-constructed `T` will be returned.
   *
   * \return Constant reference to the last element.
   * \throw std::runtime_error when there is no element in the circular_queue.
   */
  const_reference back() const;

  /**
   * \brief Checks if the underlying container has no elements.
   *
   * \return `true` if there are no elements in the queue, `false` otherwise.
   */
  DERPLIB_NODISCARD bool empty() const noexcept;

  /**
   * \brief Returns the number of elements.
   *
   * \return The number of elements in the circular_queue.
   */
  size_type size() const noexcept;

  /**
   * \brief Pushes the given element \p value to the end of the queue.
   *
   * \param[in] value Value of the element to push.
   * \throw std::length_error when the queue is full.
   */
  void push(const value_type& value);

  /**
   * \brief Pushes the given element \p value to the end of the queue.
   *
   * \param[in] value Value of the element to push.
   * \throw std::length_error when the queue is full.
   */
  void push(value_type&& value);

#if defined(DERPLIB_HAS_CPP17_SUPPORT)
  /**
   * \brief Pushes a new element to the end of the queue, which will be constructed in-place.
   *
   * \tparam Args Types as supplied to the element's constructor.
   * \param args Arguments to forward to the constructor of the element.
   * \return A reference to the pushed element. If the action fails, a reference to the last element will be returned.
   * \throw std::length_error when the queue is full.
   */
  template<typename... Args>
  decltype(auto) emplace(Args&&... args);
#else
  /**
   * \brief Pushes a new element to the end of the queue, which will be constructed in-place.
   *
   * \tparam Args Types as supplied to the element's constructor.
   * \param args Arguments to forward to the constructor of the element.
   * \throw std::length_error when the queue is full.
   */
  template<typename... Args>
  void emplace(Args&&... args);
#endif  // defined(DERPLIB_HAS_CPP17_SUPPORT)

  /**
   * \brief Removes an element from the top of the queue.
   */
  void pop() noexcept(std::is_nothrow_default_constructible<T>::value);

  /**
   * \brief Exchanges the contents of this object with `other`.
   */
  void swap(circular_queue& other) noexcept(stdext::is_nothrow_swappable<container_type>::value);

 private:
  /**
   * \brief Pointer type for the stored elements. Equivalent to `T*`.
   */
  using pointer = typename container_type::pointer;

  std::array<T, N> _data_;

  pointer _begin_ = _data_.begin();
  pointer _end_ = nullptr;

  size_type _size_ = 0;
};

/**
 * \brief Specialization of `std::swap` algorithm.
 *
 * \param lhs Containers to swap.
 * \param rhs Containers to swap.
 */
template<typename T, std::size_t N>
inline void swap(circular_queue<T, N>& lhs, circular_queue<T, N>& rhs) noexcept(
    stdext::is_nothrow_swappable<typename circular_queue<T, N>::container_type>::value) {
  lhs.swap(rhs);
}

template<typename T, std::size_t N>
template<std::size_t SIZE>
circular_queue<T, N>::circular_queue(const std::array<T, SIZE>& cont) noexcept(
    std::is_nothrow_copy_assignable<T>::value&& std::is_nothrow_default_constructible<T>::value) :
    _begin_{_data_.begin()},
    _end_{SIZE > N ? _data_.begin() + N : _data_.begin() + SIZE},
    _size_{std::min(SIZE, N)} {
  std::copy(cont.begin(), cont.begin() + _size_, _begin_);
  std::fill(_end_, _data_.end(), T());
}

template<typename T, std::size_t N>
circular_queue<T, N>::circular_queue(container_type&& cont) noexcept(
    std::is_nothrow_move_assignable<container_type>::value) :
    _data_{std::move(cont)},
    _begin_{_data_.begin()},
    _end_{_data_.end()},
    _size_{N} {}

template<typename T, std::size_t N>
circular_queue<T, N>::circular_queue(const circular_queue& other) noexcept(
    std::is_nothrow_copy_constructible<container_type>::value) :
    _data_{other._data_},
    _begin_{_data_.begin() + (other._begin_ - other._data_.begin())},
    _end_{other._end_ == nullptr ? nullptr : _data_.begin() + (other._end_ - other._data_.begin())},
    _size_{other._size_} {}

template<typename T, std::size_t N>
circular_queue<T, N>::circular_queue(circular_queue&& other) noexcept(
    std::is_nothrow_move_constructible<container_type>::value) :
    _data_{std::move(other._data_)},
    _begin_{_data_.begin() + (other._begin_ - other._data_.begin())},
    _end_{other._end_ == nullptr ? nullptr : _data_.begin() + (other._end_ - other._data_.begin())},
    _size_{other._size_} {}

template<typename T, std::size_t N>
circular_queue<T, N>& circular_queue<T, N>::operator=(const circular_queue& other) & noexcept(
    std::is_nothrow_copy_assignable<container_type>::value) {
  if (&other == this) {
    return *this;
  }

  _data_ = other._data_;
  _begin_ = _data_.begin() + (other._begin_ - other._data_.begin());
  _end_ = other._end_ == nullptr ? nullptr : _data_.begin() + (other._end_ - other._data_.begin());
  _size_ = other._size_;

  return *this;
}

template<typename T, std::size_t N>
circular_queue<T, N>& circular_queue<T, N>::operator=(circular_queue&& other) & noexcept(
    std::is_nothrow_move_assignable<container_type>::value) {
  if (&other == this) {
    return *this;
  }

  _data_ = std::move(other._data_);
  _begin_ = _data_.begin() + (other._begin_ - other._data_.begin());
  _end_ = other._end_ == nullptr ? nullptr : _data_.begin() + (other._end_ - other._data_.begin());
  _size_ = other._size_;

  return *this;
}

template<typename T, std::size_t N>
typename circular_queue<T, N>::reference circular_queue<T, N>::front() {
  if (empty()) {
    throw std::runtime_error{"front(): no element"};
  }

  return *_begin_;
}

template<typename T, std::size_t N>
typename circular_queue<T, N>::const_reference circular_queue<T, N>::front() const {
  if (empty()) {
    throw std::runtime_error{"front(): no element"};
  }

  return *_begin_;
}

template<typename T, std::size_t N>
typename circular_queue<T, N>::reference circular_queue<T, N>::back() {
  if (empty()) {
    throw std::runtime_error{"back(): no element"};
  }

  return _end_[-1];
}

template<typename T, std::size_t N>
typename circular_queue<T, N>::const_reference circular_queue<T, N>::back() const {
  if (empty()) {
    throw std::runtime_error{"back(): no element"};
  }

  return _end_[-1];
}

template<typename T, std::size_t N>
bool circular_queue<T, N>::empty() const noexcept {
  return _size_ == 0;
}

template<typename T, std::size_t N>
typename circular_queue<T, N>::size_type circular_queue<T, N>::size() const noexcept {
  return _size_;
}

template<typename T, std::size_t N>
void circular_queue<T, N>::push(const value_type& value) {
  if (size() == N) {
    throw std::length_error{"push(): max elements alloc'd"};
  }

  if (empty() || _end_ == _data_.end()) {
    _end_ = _data_.begin();
  }

  *_end_++ = value;
  ++_size_;
}

template<typename T, std::size_t N>
void circular_queue<T, N>::push(value_type&& value) {
  if (size() == N) {
    throw std::length_error{"push(): max elements alloc'd"};
  }

  if (empty() || _end_ == _data_.end()) {
    _end_ = _data_.begin();
  }

  *_end_++ = std::move(value);
  ++_size_;
}

#if defined(DERPLIB_HAS_CPP17_SUPPORT)
template<typename T, std::size_t N>
template<typename... Args>
decltype(auto) circular_queue<T, N>::emplace(Args&&... args) {
  if (size() == N) {
    throw std::length_error{"push(): max elements alloc'd"};
  }

  if (empty() || _end_ == _data_.end()) {
    _end_ = _data_.begin();
  }

  *_end_++ = T{std::forward<Args>(args)...};
  ++_size_;

  return back();
}
#else
template<typename T, std::size_t N>
template<typename... Args>
void circular_queue<T, N>::emplace(Args&&... args) {
  if (size() == N) {
    throw std::length_error{"push(): max elements alloc'd"};
  }

  if (empty() || _end_ == _data_.end()) {
    _end_ = _data_.begin();
  }

  *_end_++ = T{std::forward<Args>(args)...};
  ++_size_;
}
#endif  // defined(DERPLIB_HAS_CPP17_SUPPORT)

template<typename T, std::size_t N>
void circular_queue<T, N>::pop() noexcept(std::is_nothrow_default_constructible<T>::value) {
  if (empty()) {
    return;
  }

  *_begin_ = T{};
  ++_begin_;
  --_size_;

  if (empty() || _begin_ == _data_.end()) {
    _begin_ = _data_.begin();
  }
  if (empty()) {
    _end_ = nullptr;
  }
}

template<typename T, std::size_t N>
void circular_queue<T, N>::swap(circular_queue& other) noexcept(stdext::is_nothrow_swappable<container_type>::value) {
  std::swap(this->_data_, other._data_);
  std::swap(this->_begin_, other._begin_);
  std::swap(this->_end_, other._end_);
  std::swap(this->_size_, other._size_);
}

#include <derplib/internal/common_macros_end.h>

}  // namespace container
}  // namespace derplib
