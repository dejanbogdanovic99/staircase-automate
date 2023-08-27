#pragma once

#include <hal/BinaryValue.hxx>
#include <hal/IBinaryValueWriter.hxx>
#include <hal/Timing.hxx>

namespace staircase {

class BasicLight final {
  public:
    enum class LightState { ON, OFF };

    BasicLight(hal::IBinaryValueWriter &binaryValueWriter,
               LightState lightState = LightState::OFF,
               hal::Milliseconds millis = hal::kForever) noexcept;

    BasicLight(const BasicLight &) = delete;
    BasicLight(BasicLight &&) noexcept = default;
    BasicLight &operator=(const BasicLight &) = delete;
    BasicLight &operator=(BasicLight &&) noexcept = default;

    ~BasicLight() = default;

    void turnOn(hal::Milliseconds millis = hal::kForever) noexcept;
    void turnOff(hal::Milliseconds millis = hal::kForever) noexcept;

    void update(hal::Milliseconds delta) noexcept;

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