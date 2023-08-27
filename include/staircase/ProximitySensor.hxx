#pragma once

#include <hal/BinaryValue.hxx>
#include <hal/IBinaryValueReader.hxx>
#include <hal/Timing.hxx>

namespace staircase {

class ProximitySensor final {
  public:
    enum class SensorState { CLOSE, FAR };
    static constexpr hal::Milliseconds kDebouncePeriod = 300;

    ProximitySensor(hal::IBinaryValueReader &binaryValueReader) noexcept;

    ProximitySensor(const ProximitySensor &) = delete;
    ProximitySensor(ProximitySensor &&) noexcept = default;
    ProximitySensor &operator=(const ProximitySensor &) = delete;
    ProximitySensor &operator=(ProximitySensor &&) noexcept = default;

    ~ProximitySensor() = default;

    bool stateChanged() const noexcept;
    bool isClose() const noexcept;
    bool isFar() const noexcept;

    void update(hal::Milliseconds delta) noexcept;

  private:
    SensorState readState() noexcept;

    hal::IBinaryValueReader &mBinaryValueReader;
    SensorState mState;
    bool mStateChanged;
    hal::Milliseconds mTimePassed;
};

} // namespace staircase