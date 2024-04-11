#pragma once

#include <staircase/IMoving.hxx>
#include <staircase/IMovingDurationCalculator.hxx>
#include <staircase/IMovingFactory.hxx>
#include <staircase/Moving.hxx>

#include <algorithm>
#include <array>
#include <cstdint>

namespace staircase {

class BasicMovingFactory final : public IMovingFactory {
  public:
    BasicMovingFactory() {}
    BasicMovingFactory(const BasicMovingFactory &) noexcept = delete;
    BasicMovingFactory(BasicMovingFactory &&) noexcept = default;
    BasicMovingFactory &operator=(const BasicMovingFactory &) noexcept = delete;
    BasicMovingFactory &operator=(BasicMovingFactory &&) noexcept = default;

    ~BasicMovingFactory() = default;

    MovingPtr create(BasicLights &lights,
                     IMovingDurationCalculator &durationCalculator,
                     IMoving::Direction direction,
                     hal::Milliseconds duration) noexcept final {
        return MovingPtr{
            new Moving{lights, durationCalculator, direction, duration},
            [](IMoving *moving) { delete moving; }};
    }
};

} // namespace staircase