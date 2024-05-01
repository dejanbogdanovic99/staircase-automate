#include <staircase/StaircaseLooper.hxx>

#include <hal/Timing.hxx>

#include <staircase/IBasicLight.hxx>
#include <staircase/IMoving.hxx>
#include <staircase/IMovingDurationCalculator.hxx>
#include <staircase/IMovingFactory.hxx>
#include <staircase/IMovingTimeFilter.hxx>
#include <staircase/IProximitySensor.hxx>
#include <staircase/IStaircaseLooper.hxx>

#include <algorithm>
#include <mutex>
#include <string>

using namespace staircase;

StaircaseLooper::StaircaseLooper(BasicLights &lights,
                                 IProximitySensor &downSensor,
                                 IProximitySensor &upSensor,
                                 IMovingFactory &movingFactory,
                                 IMovingDurationCalculator &durationCalculator,
                                 IMovingTimeFilter &downMovingFilter,
                                 IMovingTimeFilter &upMovingFilter) noexcept
    : mLights{lights}, mDownSensor{downSensor}, mUpSensor{upSensor},
      mMovingFactory{movingFactory}, mDurationCalculator{durationCalculator},
      mDownMovingFilter{downMovingFilter}, mUpMovingFilter{upMovingFilter} {}

void StaircaseLooper::update(hal::Milliseconds delta) noexcept {
    std::lock_guard<std::mutex> lock{mLock};

    updateLights(delta);
    updateSensors(delta);
    updateMovigns(delta);

    removeAllStaleMovings(mDownMovings);
    removeAllStaleMovings(mUpMovings);

    if (mDownSensor.hasStateChanged() && mDownSensor.isClose()) {
        handleDownSensorStateChanged();
    }

    if (mUpSensor.hasStateChanged() && mUpSensor.isClose()) {
        handleUpSensorStateChanged();
    }
}

std::lock_guard<std::mutex> StaircaseLooper::block() noexcept {
    return std::lock_guard<std::mutex>{mLock};
}

void StaircaseLooper::updateLights(hal::Milliseconds delta) noexcept {
    std::for_each(std::begin(mLights), std::end(mLights),
                  [delta](auto light) { light.get().update(delta); });
}

void StaircaseLooper::updateSensors(hal::Milliseconds delta) noexcept {
    mDownSensor.update(delta);
    mUpSensor.update(delta);
}

void StaircaseLooper::updateMovigns(hal::Milliseconds delta) noexcept {
    std::for_each(std::begin(mDownMovings), std::end(mDownMovings),
                  [delta](auto &moving) { moving->update(delta); });

    std::for_each(std::begin(mUpMovings), std::end(mUpMovings),
                  [delta](auto &moving) { moving->update(delta); });
}

void StaircaseLooper::removeAllStaleMovings(Movings &movings) noexcept {
    while (!movings.empty() && movings.front()->isTooOld()) {
        movings.pop_front();
    }
}

void StaircaseLooper::handleDownSensorStateChanged() {
    if (isFirstMovingFinishing(mDownMovings)) {
        finishFirstMoving(mDownMovings, mDownMovingFilter);
    } else if (!hasNewMovingJustStarted(mUpMovings) &&
               isMoreNewMovingsAvailable(mUpMovings)) {
        auto moving = mMovingFactory.create(
            mLights, mDurationCalculator, IMoving::Direction::UP,
            mUpMovingFilter.getCurrentMovingTime());
        if (moving) {
            mUpMovings.push_back(std::move(moving));
        }
    }
}

void StaircaseLooper::handleUpSensorStateChanged() {
    if (isFirstMovingFinishing(mUpMovings)) {
        finishFirstMoving(mUpMovings, mUpMovingFilter);
    } else if (!hasNewMovingJustStarted(mDownMovings) &&
               isMoreNewMovingsAvailable(mDownMovings)) {
        auto moving = mMovingFactory.create(
            mLights, mDurationCalculator, IMoving::Direction::DOWN,
            mDownMovingFilter.getCurrentMovingTime());
        if (moving) {
            mDownMovings.push_back(std::move(moving));
        }
    }
}

bool StaircaseLooper::isFirstMovingFinishing(Movings &movings) const noexcept {
    if (movings.empty()) {
        return false;
    }

    return movings.front()->isNearEnd();
}

bool StaircaseLooper::hasNewMovingJustStarted(Movings &movings) const noexcept {
    if (movings.empty()) {
        return false;
    }

    return movings.back()->isNearBegin();
}

bool StaircaseLooper::isMoreNewMovingsAvailable(
    Movings &movings) const noexcept {
    return movings.size() < IMoving::kMaxMovings;
}

void StaircaseLooper::finishFirstMoving(Movings &movings,
                                        IMovingTimeFilter &filter) noexcept {
    if (movings.empty()) {
        return;
    }

    auto currentDuration = movings.front()->getTimePassed();
    movings.pop_front();

    filter.processNewMovingTime(currentDuration);
}