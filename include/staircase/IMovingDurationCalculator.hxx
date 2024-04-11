#pragma once

#include <hal/Timing.hxx>

#include <cstdint>

namespace staircase {
class IMovingDurationCalculator {
  public:
    virtual ~IMovingDurationCalculator() = default;
    virtual hal::Milliseconds
    calculateDelta(std::size_t lightIndex,
                   hal::Milliseconds totalDuration) const noexcept = 0;
};
} // namespace staircase
