#pragma once

#include <hal/Timing.hxx>

#include <staircase/IBasicLight.hxx>
#include <staircase/IMoving.hxx>

namespace staircase {

class IMovingFactory {
  public:
    virtual IMoving *create(BasicLights lights, IMoving::Direction direction,
                            hal::Milliseconds duration) noexcept = 0;
    virtual void destroy(IMoving *moving) noexcept = 0;
};

} // namespace staircase