#pragma once

#include <algorithm>
#include <memory>
#include <vector>

#if __cplusplus > 199711L
namespace derplib {
template<class T, class Deleter = std::default_delete<T>>
class ref_count_pointer {
 public:
  using pointer = typename std::unique_ptr<T, Deleter>::pointer;
  using element_type = typename std::unique_ptr<T, Deleter>::element_type;
  using deleter_type = typename std::unique_ptr<T, Deleter>::deleter_type;

  class observer {
   public:
    using pointer = ref_count_pointer::pointer;
    using element_type = ref_count_pointer::element_type;

    observer(element_type* p, ref_count_pointer<T>* src) noexcept : _p(p), _rcp(src) {}

    observer(const observer& other) : _p(other._p), _rcp(other._rcp) {
      _rcp->_v.push_back(this);
    }

    observer(observer&& other) : _p(other._p), _rcp(other._rcp) {
      other._p = nullptr;
      other._rcp = nullptr;

      _rcp->_v.push_back(this);
    }

    typename std::add_lvalue_reference<T>::type operator*() const { return *_p; }
    pointer operator->() const noexcept { return _p; }
    T* get() const { return _p; }

    ~observer() {
      _rcp->_v.erase(std::remove(_rcp->_v.begin(), _rcp->_v.end(), this));
    }

   private:
    T* _p;
    ref_count_pointer<T>* _rcp;

    friend class ref_count_pointer;
  };

  constexpr ref_count_pointer() noexcept : _u(), _v() {}

  constexpr ref_count_pointer(std::nullptr_t) noexcept : _u(std::nullptr_t()), _v() {}

  explicit ref_count_pointer(pointer p) noexcept : _u(p), _v() {}

  // TODO: Overload 3, 4 for unique_ptr
  ref_count_pointer(ref_count_pointer&& rc) noexcept : _u(std::move(rc._u)), _v(std::move(rc._v)) {}

  template<typename U, typename E>
  ref_count_pointer(ref_count_pointer<U, E>&& u) noexcept : _u(std::move(u._u)), _v(std::move(u._v)) {}

  ~ref_count_pointer() {
    std::for_each(_v.begin(), _v.end(), [&](observer*& o) {
      o->_p = nullptr;
      o->_rcp = nullptr;
    });
  }

  ref_count_pointer& operator=(ref_count_pointer&& r) noexcept {
    if (this == &r) {
      return *this;
    }

    _u = std::move(r._u);
    _v = std::move(r._v);

    return *this;
  }

  ref_count_pointer& operator=(std::nullptr_t) noexcept {
    _u = std::unique_ptr<T>();
    _v = std::vector<observer*>();

    return *this;
  }

  pointer release() noexcept {
    _u.release();

    std::for_each(_v.begin(), _v.end(), [](observer*& p) { p->_p = nullptr; });
    _v.clear();
  }

  void reset(pointer ptr = pointer()) noexcept {
    _u.reset(ptr);

    std::for_each(_v.begin(), _v.end(), [&](observer*& p) { p->_p = nullptr; });
    _v.clear();
  }

  observer get() noexcept {
    observer p = observer(_u.get(), this);
    _v.push_back(&p);
    return p;
  }

 private:
  std::unique_ptr<T, Deleter> _u;
  std::vector<observer*> _v;
};
}  // namespace derplib
#else
#error "This library requires C++11 or above."
#endif  // __cplusplus > 199711L
