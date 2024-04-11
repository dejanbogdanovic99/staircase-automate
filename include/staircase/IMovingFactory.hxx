#pragma once

#include <hal/Timing.hxx>

#include <staircase/IBasicLight.hxx>
#include <staircase/IMoving.hxx>
#include <staircase/IMovingDurationCalculator.hxx>

#include <memory>

namespace staircase {

class IMovingFactory {
  public:
    virtual ~IMovingFactory() = default;
    virtual MovingPtr create(BasicLights &lights,
                             IMovingDurationCalculator &durationCalculator,
                             IMoving::Direction direction,
                             hal::Milliseconds duration) noexcept = 0;
};

} // namespace staircase