#pragma once

#include <hal/Timing.hxx>

namespace staircase {

class IMoving {
  public:
    enum class Direction { UP, DOWN };

    virtual void update(hal::Milliseconds delta) noexcept = 0;
    virtual hal::Milliseconds getTimePassed() const noexcept = 0;
    virtual bool isCompleted() const noexcept = 0;
    virtual bool isNearEnd() const noexcept = 0;
    virtual bool isNearBegin() const noexcept = 0;
    virtual void complete() noexcept = 0;
};

} // namespace staircase