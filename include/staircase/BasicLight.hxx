#pragma once

#include <hal/BinaryValue.hxx>
#include <hal/IBinaryValueWriter.hxx>
#include <hal/Timing.hxx>

#include <staircase/IBasicLight.hxx>

namespace staircase {

class BasicLight final : public IBasicLight {
  public:
    BasicLight(hal::IBinaryValueWriter &binaryValueWriter) noexcept;

    BasicLight(const BasicLight &) = delete;
    BasicLight(BasicLight &&) noexcept = delete;
    BasicLight &operator=(const BasicLight &) = delete;
    BasicLight &operator=(BasicLight &&) noexcept = delete;

    ~BasicLight() = default;

    void turnOn(hal::Milliseconds millis = DEFAULT_ON_PERIOD) noexcept final;
    void turnOff() noexcept final;
    void update(hal::Milliseconds delta) noexcept final;

    bool isOn() const noexcept final;
    bool isOff() const noexcept final;

  private:
    enum class LightState { OFF, ON };

    void setState(LightState state, hal::Milliseconds millis) noexcept;
    void writeState() noexcept;

    hal::IBinaryValueWriter &mBinaryValueWriter;
    LightState mState;
    hal::Milliseconds mTimeLeft;
};

} // namespace staircase