#pragma once

#include <algorithm>
#include <memory>
#include <vector>

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

    observer(element_type* p, ref_count_pointer<T>* src) noexcept : _p_{p}, _rcp_{src} {}

    observer(const observer& other) : _p_{other._p_}, _rcp_{other._rcp_} { _rcp_->_v_.push_back(this); }

    observer(observer&& other) : _p_{other._p_}, _rcp_{other._rcp_} {
      other._p_ = nullptr;
      other._rcp_ = nullptr;

      _rcp_->_v_.push_back(this);
    }

    typename std::add_lvalue_reference<T>::type operator*() const { return *_p_; }
    pointer operator->() const noexcept { return _p_; }
    T* get() const { return _p_; }

    ~observer() { _rcp_->_v_.erase(std::remove(_rcp_->_v_.begin(), _rcp_->_v_.end(), this)); }

   private:
    T* _p_;
    ref_count_pointer<T>* _rcp_;

    friend class ref_count_pointer;
  };

  constexpr ref_count_pointer() noexcept : _u_{}, _v_{} {}

  constexpr ref_count_pointer(std::nullptr_t) noexcept : _u_{std::nullptr_t{}}, _v_{} {}

  explicit ref_count_pointer(pointer p) noexcept : _u_{p}, _v_{} {}

  // TODO: Overload 3, 4 for unique_ptr
  ref_count_pointer(ref_count_pointer&& rc) noexcept : _u_{std::move(rc._u_)}, _v_{std::move(rc._v_)} {}

  template<typename U, typename E>
  ref_count_pointer(ref_count_pointer<U, E>&& u) noexcept : _u_{std::move(u._u_)}, _v_{std::move(u._v_)} {}

  ~ref_count_pointer() {
    // TODO: We don't need the reference capture right?
    std::for_each(_v_.begin(), _v_.end(), [&](observer*& o) {
      o->_p_ = nullptr;
      o->_rcp_ = nullptr;
    });
  }

  ref_count_pointer& operator=(ref_count_pointer&& r) noexcept {
    if (this == &r) {
      return *this;
    }

    _u_ = std::move(r._u_);
    _v_ = std::move(r._v_);

    return *this;
  }

  ref_count_pointer& operator=(std::nullptr_t) noexcept {
    _u_ = std::unique_ptr<T>{};
    _v_ = std::vector<observer*>{};

    return *this;
  }

  pointer release() noexcept {
    _u_.release();

    std::for_each(_v_.begin(), _v_.end(), [](observer*& p) { p->_p_ = nullptr; });
    _v_.clear();
  }

  void reset(pointer ptr = pointer()) noexcept {
    _u_.reset(ptr);

    // TODO: We don't need to lambda capture right?
    std::for_each(_v_.begin(), _v_.end(), [&](observer*& p) { p->_p_ = nullptr; });
    _v_.clear();
  }

  observer get() noexcept {
    // TODO: auto-init
    observer p = observer{_u_.get(), this};
    _v_.push_back(&p);
    return p;
  }

 private:
  std::unique_ptr<T, Deleter> _u_;
  std::vector<observer*> _v_;
};
}  // namespace derplib
