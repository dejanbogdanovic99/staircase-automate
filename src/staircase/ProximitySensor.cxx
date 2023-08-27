#include <staircase/ProximitySensor.hxx>

#include <hal/BinaryValue.hxx>
#include <hal/IBinaryValueReader.hxx>
#include <hal/Timing.hxx>

using namespace staircase;

ProximitySensor::ProximitySensor(
    hal::IBinaryValueReader &binaryValueReader) noexcept
    : mBinaryValueReader{binaryValueReader}, mState{readState()},
      mStateChanged{false}, mTimePassed{hal::kForever} {}

bool ProximitySensor::stateChanged() const noexcept { return mStateChanged; }

bool ProximitySensor::isClose() const noexcept {
    return mState == SensorState::CLOSE;
}

bool ProximitySensor::isFar() const noexcept {
    return mState == SensorState::FAR;
}

void ProximitySensor::update(hal::Milliseconds delta) noexcept {
    mStateChanged = false;

    auto newState = readState();

    if (newState != mState) {
        if (mTimePassed == hal::kForever) {
            mTimePassed = delta / 2;
        } else {
            mTimePassed += delta;
        }

        if (mTimePassed >= kDebouncePeriod) {
            mState = newState;
            mStateChanged = true;
            mTimePassed = hal::kForever;
        }
    }
}

ProximitySensor::SensorState ProximitySensor::readState() noexcept {
    auto binaryValue = mBinaryValueReader.readValue();

    switch (binaryValue) {
    case hal::BinaryValue::HIGH:
        return SensorState::CLOSE;
    case hal::BinaryValue::LOW:
    default:
        return SensorState::FAR;
    }
}