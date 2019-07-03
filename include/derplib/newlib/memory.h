#pragma once

#include <memory>

namespace derplib {
namespace newlib {

template<typename T, typename Deleter>
T* element_at(const std::unique_ptr<T[], Deleter>& uptr, std::size_t index) noexcept;

}  // namespace newlib
}  // namespace derplib

#include "derplib/newlib/memory.ipp"
