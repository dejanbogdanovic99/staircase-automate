#pragma once

#include <hal/Timing.hxx>

namespace staircase {

class IStaircaseLooper {
  public:
    virtual void update(hal::Milliseconds delta) noexcept = 0;
};

} // namespace staircase