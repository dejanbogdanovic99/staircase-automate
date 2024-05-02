#pragma once

#include <hal/Timing.hxx>

#include <staircase/IMovingDurationCalculator.hxx>

#include <cstdint>

namespace staircase {
class ClippedSquaredMovingDurationCalculator final
    : public IMovingDurationCalculator {
  public:
    hal::Milliseconds
    calculateDelta(std::size_t lightIndex,
                   hal::Milliseconds totalDuration) const noexcept override;

  private:
    static constexpr hal::Milliseconds kFirstLightDelta = 500;
    static constexpr hal::Milliseconds kSecondLightDelta = 750;
    static constexpr hal::Milliseconds kThirdLightDelta = 1200;
};
} // namespace staircase