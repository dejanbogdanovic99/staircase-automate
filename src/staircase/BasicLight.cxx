#include <staircase/BasicLight.hxx>

#include <hal/BinaryValue.hxx>
#include <hal/IBinaryValueWriter.hxx>
#include <hal/Timing.hxx>

#include <staircase/IBasicLight.hxx>

#include <algorithm>

using namespace staircase;

BasicLight::BasicLight(hal::IBinaryValueWriter &binaryValueWriter) noexcept
    : mBinaryValueWriter{binaryValueWriter}, mState{LightState::OFF},
      mTimeLeft{hal::kForever} {
    writeState();
}

void BasicLight::turnOn(hal::Milliseconds millis) noexcept {
    setState(LightState::ON, millis);
}

void BasicLight::turnOff() noexcept {
    setState(LightState::OFF, hal::kForever);
}

void BasicLight::update(hal::Milliseconds delta) noexcept {
    if (mTimeLeft < 0) {
        return;
    }

    if (delta >= mTimeLeft) {
        setState(LightState::OFF, hal::kForever);
    } else {
        mTimeLeft -= delta;
    }
}

bool BasicLight::isOn() const noexcept { return mState == LightState::ON; }

bool BasicLight::isOff() const noexcept { return mState == LightState::OFF; }

void BasicLight::setState(LightState state, hal::Milliseconds millis) noexcept {
    if (mState != state) {
        mState = state;
        writeState();
    }

    if (millis == hal::kForever) {
        mTimeLeft = millis;
    } else {
        mTimeLeft = std::max(mTimeLeft, millis);
    }
}

void BasicLight::writeState() noexcept {
    auto binaryValue = hal::BinaryValue::LOW;

    switch (mState) {
    case LightState::ON:
        binaryValue = hal::BinaryValue::HIGH;
        break;
    case LightState::OFF:
    default:
        binaryValue = hal::BinaryValue::LOW;
        break;
    }

    mBinaryValueWriter.writeValue(binaryValue);
}