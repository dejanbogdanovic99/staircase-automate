#pragma once

#include <hal/BinaryValue.hxx>
#include <hal/IBinaryValueWriter.hxx>
#include <hal/Timing.hxx>

#include <staircase/IBasicLight.hxx>

namespace staircase {

class BasicLight final : public IBasicLight {
  public:
    enum class LightState { ON, OFF };

    BasicLight(hal::IBinaryValueWriter &binaryValueWriter,
               LightState lightState = LightState::OFF,
               hal::Milliseconds millis = hal::kForever) noexcept;

    BasicLight(const BasicLight &) = delete;
    BasicLight(BasicLight &&) noexcept = delete;
    BasicLight &operator=(const BasicLight &) = delete;
    BasicLight &operator=(BasicLight &&) noexcept = delete;

    ~BasicLight() = default;

    void turnOn(hal::Milliseconds millis = DEFAULT_ON_PERIOD) noexcept final;
    void update(hal::Milliseconds delta) noexcept final;

    LightState getState() const noexcept;

  private:
    void setState(LightState state, hal::Milliseconds millis) noexcept;
    void writeState() noexcept;
    void changeToOtherState() noexcept;

    hal::IBinaryValueWriter &mBinaryValueWriter;
    LightState mState;
    hal::Milliseconds mTimeLeft;
};

} // namespace staircase