#pragma once

#include <cstdint>

namespace hal {

using Milliseconds = std::int32_t;
constexpr Milliseconds kForever = -1;

} // namespace hal