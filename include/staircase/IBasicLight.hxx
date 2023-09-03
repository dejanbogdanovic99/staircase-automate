#pragma once

#include <hal/Timing.hxx>

namespace staircase {

class IBasicLight {
  public:
    virtual ~IBasicLight() = default;
    virtual void turnOn(hal::Milliseconds millis) noexcept = 0;
    virtual void turnOff(hal::Milliseconds millis) noexcept = 0;
    virtual void update(hal::Milliseconds delta) noexcept = 0;
};

} // namespace staircase