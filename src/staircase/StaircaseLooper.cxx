#include <staircase/StaircaseLooper.hxx>

#include <hal/Timing.hxx>

#include <staircase/IBasicLight.hxx>
#include <staircase/IMoving.hxx>
#include <staircase/IMovingFactory.hxx>
#include <staircase/IProximitySensor.hxx>

#include <algorithm>
#include <mutex>

using namespace staircase;

StaircaseLooper::StaircaseLooper(BasicLights &lights,
                                 IProximitySensor &downSensor,
                                 IProximitySensor &upSensor,
                                 IMovingFactory &movingFactory) noexcept
    : mLights{lights}, mDownSensor{downSensor}, mUpSensor{upSensor},
      mMovingFactory{movingFactory},
      mDownMovingDuration{kInitialMovingDuration},
      mUpMovingDuration{kInitialMovingDuration} {}

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
        finishFirstMoving(mDownMovings, mDownMovingDuration);
    } else if (!hasNewMovingJustStarted(mUpMovings) &&
               isMoreNewMovingsAvailable(mUpMovings)) {
        auto moving = mMovingFactory.create(mLights, IMoving::Direction::UP,
                                            mUpMovingDuration);
        if (moving) {
            mUpMovings.push_back(std::move(moving));
        }
    }
}

void StaircaseLooper::handleUpSensorStateChanged() {
    if (isFirstMovingFinishing(mUpMovings)) {
        finishFirstMoving(mUpMovings, mUpMovingDuration);
    } else if (!hasNewMovingJustStarted(mDownMovings) &&
               isMoreNewMovingsAvailable(mDownMovings)) {
        auto moving = mMovingFactory.create(mLights, IMoving::Direction::DOWN,
                                            mDownMovingDuration);
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
                                        hal::Milliseconds &duration) noexcept {
    if (movings.empty()) {
        return;
    }

    auto currentDuration = movings.front()->getTimePassed();
    movings.pop_front();

    // TODO fix duration calculation
    duration = (duration + currentDuration) / 2;
}