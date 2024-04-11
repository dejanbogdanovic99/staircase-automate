#include <staircase/MTAMovingTimeFilter.hxx>

#include <hal/Timing.hxx>

#include <algorithm>
#include <cstdint>
#include <numeric>

using namespace staircase;

MTAMovingTimeFilter::MTAMovingTimeFilter(hal::Milliseconds initialFilterValue)
    : mFilterValues{initialFilterValue, initialFilterValue, initialFilterValue,
                    initialFilterValue, initialFilterValue},
      mCurrentIndex{0}, mCurrentMovingTime{initialFilterValue} {}

hal::Milliseconds MTAMovingTimeFilter::getCurrentMovingTime() const noexcept {
    return mCurrentMovingTime;
}

void MTAMovingTimeFilter::processNewMovingTime(
    hal::Milliseconds timeElapsed) noexcept {
    mFilterValues[mCurrentIndex] = timeElapsed;

    mCurrentIndex++;
    if (mCurrentIndex == mFilterValues.size()) {
        mCurrentIndex = 0;
    }

    hal::Milliseconds sum =
        std::accumulate(std::begin(mFilterValues), std::end(mFilterValues), 0);
    mCurrentMovingTime = sum / mFilterValues.size();
}

void MTAMovingTimeFilter::reset(hal::Milliseconds timeElapsed) noexcept {
    std::fill(std::begin(mFilterValues), std::end(mFilterValues), timeElapsed);
    mCurrentMovingTime = timeElapsed;
    mCurrentIndex = 0;
}