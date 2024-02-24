#pragma once

#include <staircase/IMoving.hxx>
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

    IMoving *create(BasicLights lights, IMoving::Direction direction,
                    hal::Milliseconds duration) noexcept final {
        return new Moving{lights, direction, duration};
    }
    void destroy(IMoving *moving) noexcept final { delete moving; }
};

} // namespace staircase