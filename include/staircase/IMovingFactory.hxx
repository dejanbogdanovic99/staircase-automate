#pragma once

#include <hal/Timing.hxx>

#include <staircase/IBasicLight.hxx>
#include <staircase/IMoving.hxx>

#include <memory>

namespace staircase {

class IMovingFactory {
  public:
    virtual ~IMovingFactory() = default;
    virtual MovingPtr create(BasicLights &lights, IMoving::Direction direction,
                             hal::Milliseconds duration) noexcept = 0;
};

} // namespace staircase