#pragma once

#include <hal/Timing.hxx>

#include <cstdint>
#include <functional>
#include <span>

namespace staircase {

class IBasicLight {
  public:
    static constexpr std::size_t kLightsNum = LIGHTS_NUM;
    static constexpr hal::Milliseconds kDefaultOnPeriod = DEFAULT_ON_PERIOD;

    virtual ~IBasicLight() = default;
    virtual void
    turnOn(hal::Milliseconds millis = kDefaultOnPeriod) noexcept = 0;
    virtual void turnOff() noexcept = 0;
    virtual void update(hal::Milliseconds delta) noexcept = 0;
    virtual bool isOn() const noexcept = 0;
    virtual bool isOff() const noexcept = 0;
};

using BasicLights =
    std::array<std::reference_wrapper<IBasicLight>, IBasicLight::kLightsNum>;

} // namespace staircase