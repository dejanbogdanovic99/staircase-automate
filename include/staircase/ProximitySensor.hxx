#pragma once

#include <hal/IBinaryValueReader.hxx>
#include <hal/Timing.hxx>

#include <staircase/IProximitySensor.hxx>

namespace staircase {

class ProximitySensor final : public IProximitySensor {
  public:
    enum class SensorState { CLOSE, FAR };

    ProximitySensor(hal::IBinaryValueReader &binaryValueReader) noexcept;

    ProximitySensor(const ProximitySensor &) = delete;
    ProximitySensor(ProximitySensor &&) noexcept = delete;
    ProximitySensor &operator=(const ProximitySensor &) = delete;
    ProximitySensor &operator=(ProximitySensor &&) noexcept = delete;

    ~ProximitySensor() = default;

    bool hasStateChanged() const noexcept final;
    bool isClose() const noexcept final;
    bool isFar() const noexcept final;

    void update(hal::Milliseconds delta) noexcept final;

  private:
    static constexpr hal::Milliseconds kDebouncePeriod = DEBOUNCE_PERIOD;

    SensorState readState() noexcept;

    hal::IBinaryValueReader &mBinaryValueReader;
    SensorState mState;
    bool mStateChanged;
    hal::Milliseconds mTimePassed;
};

} // namespace staircase