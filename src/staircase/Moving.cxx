#include <staircase/Moving.hxx>

#include <hal/Timing.hxx>

#include <staircase/IBasicLight.hxx>
#include <staircase/IMovingDurationCalculator.hxx>

#include <algorithm>
#include <cstdint>
#include <memory>
#include <span>

using namespace staircase;

Moving::Moving(BasicLights &lights,
               IMovingDurationCalculator &durationCalculator,
               Direction direction, hal::Milliseconds duration) noexcept
    : mLights{lights}, mDurationCalculator{durationCalculator},
      mCurrentIndex{0}, mCompleted{false}, mDirection{direction},
      mExpectedDuration{duration},
      mTimeLeftUntilUpdate{
          mDurationCalculator.calculateDelta(mCurrentIndex, mExpectedDuration)},
      mTimePassed{0} {
    turnCurrentOn();
}

void Moving::update(hal::Milliseconds delta) noexcept {
    mTimePassed += delta;
    if (mCompleted) {
        return;
    }

    while (delta >= mTimeLeftUntilUpdate) {
        delta -= mTimeLeftUntilUpdate;

        ++mCurrentIndex;

        if (mCurrentIndex == mLights.size()) {
            mCompleted = true;
            return;
        }

        mTimeLeftUntilUpdate = mDurationCalculator.calculateDelta(
            mCurrentIndex, mExpectedDuration);

        turnCurrentOn();
    }

    mTimeLeftUntilUpdate -= delta;
}

hal::Milliseconds Moving::getTimePassed() const noexcept { return mTimePassed; }

bool Moving::isCompleted() const noexcept { return mCompleted; }

bool Moving::isNearEnd() const noexcept {
    return std::abs(mExpectedDuration - mTimePassed) < kCloseFinishDiff;
}

bool Moving::isNearBegin() const noexcept {
    return mTimePassed < kCloseFinishDiff;
}

bool Moving::isTooOld() const noexcept {
    return mTimePassed > (mExpectedDuration + kCloseFinishDiff);
}

void Moving::turnCurrentOn() noexcept {
    std::size_t currentIndex = 0;

    switch (mDirection) {
    case Direction::DOWN:
        currentIndex = mLights.size() - 1 - mCurrentIndex;
        break;
    case Direction::UP:
        currentIndex = mCurrentIndex;
        break;
    default:
        currentIndex = 0;
    }

    mLights[currentIndex].get().turnOn();
}
