#pragma once

#include <hal/Timing.hxx>

#include <cstdint>
#include <span>

namespace staircase {

class IBasicLight {
  public:
    static constexpr std::size_t kLightNum = 8;
    using BasicLights = std::span<IBasicLight *, kLightNum>;

    virtual ~IBasicLight() = default;
    virtual void turnOn(hal::Milliseconds millis) noexcept = 0;
    virtual void turnOff(hal::Milliseconds millis) noexcept = 0;
    virtual void update(hal::Milliseconds delta) noexcept = 0;
};

} // namespace staircase