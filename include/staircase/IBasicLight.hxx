#pragma once

#include <hal/Timing.hxx>

#include <cstdint>
#include <span>

namespace staircase {

class IBasicLight {
  public:
    enum class LightState { OFF, ON };

    static constexpr std::size_t kLightsNum = LIGHTS_NUM;

    virtual ~IBasicLight() = default;
    virtual void
    turnOn(hal::Milliseconds millis = DEFAULT_ON_PERIOD) noexcept = 0;
    virtual void update(hal::Milliseconds delta) noexcept = 0;
    virtual LightState getState() const noexcept = 0;
};

using BasicLights = std::array<IBasicLight *, IBasicLight::kLightsNum>;

} // namespace staircase