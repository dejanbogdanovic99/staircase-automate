#include <staircase/BasicLight.hxx>

#include <hal/BinaryValue.hxx>
#include <hal/IBinaryValueWriter.hxx>
#include <hal/Timing.hxx>

#include <staircase/IBasicLight.hxx>

#include <algorithm>

using namespace staircase;

BasicLight::BasicLight(hal::IBinaryValueWriter &binaryValueWriter,
                       LightState lightState, hal::Milliseconds millis) noexcept
    : mBinaryValueWriter{binaryValueWriter}, mState{lightState},
      mTimeLeft{millis} {
    writeState();
}

void BasicLight::turnOn(hal::Milliseconds millis) noexcept {
    setState(LightState::ON, millis);
}

void BasicLight::update(hal::Milliseconds delta) noexcept {
    if (mTimeLeft < 0) {
        return;
    }

    if (delta >= mTimeLeft) {
        mTimeLeft = hal::kForever;
        changeToOtherState();
        writeState();
    } else {
        mTimeLeft -= delta;
    }
}

BasicLight::LightState BasicLight::getState() const noexcept { return mState; }

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

void BasicLight::changeToOtherState() noexcept {
    switch (mState) {
    case LightState::ON:
        mState = LightState::OFF;
        break;
    case LightState::OFF:
    default:
        mState = LightState::ON;
        break;
    }
}