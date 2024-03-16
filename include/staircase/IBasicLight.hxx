#pragma once

#include <hal/Timing.hxx>

#include <cstdint>
#include <functional>
#include <span>

namespace staircase {

class IBasicLight {
  public:
    static constexpr std::size_t kLightsNum = LIGHTS_NUM;

    virtual ~IBasicLight() = default;
    virtual void
    turnOn(hal::Milliseconds millis = DEFAULT_ON_PERIOD) noexcept = 0;
    virtual void turnOff() noexcept = 0;
    virtual void update(hal::Milliseconds delta) noexcept = 0;
    virtual bool isOn() const noexcept = 0;
    virtual bool isOff() const noexcept = 0;
};

using BasicLights =
    std::array<std::reference_wrapper<IBasicLight>, IBasicLight::kLightsNum>;

} // namespace staircase