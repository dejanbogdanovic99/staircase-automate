#include <staircase/StaircaseLooper.hxx>

#include <hal/Timing.hxx>

#include <staircase/IBasicLight.hxx>
#include <staircase/IMoving.hxx>
#include <staircase/IMovingFactory.hxx>
#include <staircase/IProximitySensor.hxx>

#include <algorithm>

using namespace staircase;

StaircaseLooper::StaircaseLooper(BasicLights &lights,
                                 IProximitySensor &downSensor,
                                 IProximitySensor &upSensor,
                                 IMovingFactory &movingFactory) noexcept
    : mLights{lights}, mDownSensor{downSensor}, mUpSensor{upSensor},
      mMovingFactory{movingFactory},
      mDownMovingDuration{kInitialDownMovingDuration},
      mUpMovingDuration{kInitialUpMovingDuration} {}

void StaircaseLooper::run(hal::ITiming &timing,
                          hal::Milliseconds period) noexcept {
    while (true) {
        update(timing.getDelta());
        timing.sleepFor(period);
    }
}

void StaircaseLooper::update(hal::Milliseconds delta) noexcept {

    updateLights(delta);
    updateSensors(delta);
    updateMovigns(delta);

    removeAllStaleMovings(mDownMovings);
    removeAllStaleMovings(mUpMovings);

    if (mDownSensor.hasStateChanged()) {
        handleDownSensorStateChanged();
    }

    if (mUpSensor.hasStateChanged()) {
        handleUpSensorStateChanged();
    }
}

void StaircaseLooper::updateLights(hal::Milliseconds delta) noexcept {
    std::for_each(std::begin(mLights), std::end(mLights),
                  [delta](auto light) { light->update(delta); });
}

void StaircaseLooper::updateSensors(hal::Milliseconds delta) noexcept {
    mDownSensor.update(delta);
    mUpSensor.update(delta);
}

void StaircaseLooper::updateMovigns(hal::Milliseconds delta) noexcept {
    std::for_each(std::begin(mDownMovings), std::end(mDownMovings),
                  [delta](auto moving) { moving->update(delta); });

    std::for_each(std::begin(mUpMovings), std::end(mUpMovings),
                  [delta](auto moving) { moving->update(delta); });
}

void StaircaseLooper::removeAllStaleMovings(Movings &movings) noexcept {
    while (!movings.empty() && movings.front()->isTooOld()) {
        mMovingFactory.destroy(movings.front());
        movings.pop_front();
    }
}

void StaircaseLooper::handleDownSensorStateChanged() {
    if (mDownSensor.isClose()) {
        if (isFirstMovingFinishing(mDownMovings)) {
            finishFirstMoving(mDownMovings, mDownMovingDuration);
        } else if (!hasNewMovingJustStarted(mUpMovings) &&
                   mUpMovings.size() < IMoving::kMaxMovings) {
            mUpMovings.push_back(mMovingFactory.create(
                mLights, IMoving::Direction::UP, mUpMovingDuration));
        } else {
            // IGNORE
        }
    }
}

void StaircaseLooper::handleUpSensorStateChanged() {
    if (mUpSensor.isClose()) {
        if (isFirstMovingFinishing(mUpMovings)) {
            finishFirstMoving(mUpMovings, mUpMovingDuration);
        } else if (!hasNewMovingJustStarted(mDownMovings) &&
                   mDownMovings.size() < IMoving::kMaxMovings) {
            mDownMovings.push_back(mMovingFactory.create(
                mLights, IMoving::Direction::DOWN, mDownMovingDuration));
        } else {
            // IGNORE
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

void StaircaseLooper::finishFirstMoving(Movings &movings,
                                        hal::Milliseconds &duration) noexcept {
    if (movings.empty()) {
        return;
    }

    auto moving = movings.front();
    movings.pop_front();
    auto currentDuration = moving->getTimePassed();
    mMovingFactory.destroy(moving);

    // TODO fix duration calculation
    duration = (duration + currentDuration) / 2;
}