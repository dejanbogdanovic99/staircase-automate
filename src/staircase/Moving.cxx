#include <staircase/Moving.hxx>

#include <hal/Timing.hxx>

#include <algorithm>
#include <cstdint>
#include <memory>
#include <span>

using namespace staircase;

Moving::Moving(Lights lights, Direction direction,
               hal::Milliseconds duration) noexcept
    : mLights{std::move(lights)}, mCurrentLight{std::begin(mLights)},
      mCurrentIndex{0}, mCompleted{false}, mDirection{direction},
      mDuration{duration}, mStandardOnDuration{static_cast<hal::Milliseconds>(
                               mDuration / (kLightNum + 1))},
      mTimeLeftUntillUpdate{calculateInterval()}, mTimePassed{0} {
    (*mCurrentLight)->turnOn(mTimeLeftUntillUpdate);
}

void Moving::update(hal::Milliseconds delta) noexcept {
    mTimePassed += delta;
    if (mCompleted) {
        return;
    }

    while (delta >= mTimeLeftUntillUpdate) {
        delta -= mTimeLeftUntillUpdate;

        ++mCurrentLight;
        ++mCurrentIndex;

        if (mCurrentLight == std::end(mLights)) {
            mCompleted = true;
            return;
        }

        mTimeLeftUntillUpdate = calculateInterval();

        (*mCurrentLight)->turnOn(mTimeLeftUntillUpdate);
    }

    mTimeLeftUntillUpdate -= delta;
}

hal::Milliseconds Moving::getTimePassed() const noexcept { return mTimePassed; }

bool Moving::isCompleted() const noexcept { return mCompleted; }

bool Moving::isNearEnd() const noexcept {
    return std::abs(mDuration - mTimePassed) < kCloseFinishDiff;
}

bool Moving::isNearBegin() const noexcept {
    return mTimePassed < kCloseFinishDiff;
}

void Moving::complete() noexcept { mCompleted = true; }

hal::Milliseconds Moving::calculateInterval() const noexcept {
    if (mCurrentIndex < kFastRiseCount) {
        return kFastRiseTime;
    } else {
        return mStandardOnDuration;
    }
}