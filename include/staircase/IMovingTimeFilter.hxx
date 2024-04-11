#pragma once

#include <hal/Timing.hxx>

namespace staircase {

class IMovingTimeFilter {
  public:
    virtual ~IMovingTimeFilter() = default;
    virtual hal::Milliseconds getCurrentMovingTime() const noexcept = 0;
    virtual void
    processNewMovingTime(hal::Milliseconds timeElapsed) noexcept = 0;
    virtual void reset(hal::Milliseconds timeElapsed) noexcept = 0;
};
} // namespace staircase