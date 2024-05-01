#pragma once

#include <hal/Timing.hxx>

#include <staircase/IBasicLight.hxx>
#include <staircase/IMoving.hxx>
#include <staircase/IMovingDurationCalculator.hxx>
#include <staircase/IMovingFactory.hxx>
#include <staircase/IMovingTimeFilter.hxx>
#include <staircase/IProximitySensor.hxx>
#include <staircase/IStaircaseLooper.hxx>

#include <util/StaticDequeue.hxx>

#include <cstdint>
#include <memory>
#include <mutex>
#include <string>

namespace staircase {

class StaircaseLooper final : public IStaircaseLooper {
  public:
    StaircaseLooper(BasicLights &lights, IProximitySensor &downSensor,
                    IProximitySensor &upSensor, IMovingFactory &movingFactory,
                    IMovingDurationCalculator &durationCalculator,
                    IMovingTimeFilter &downMovingFilter,
                    IMovingTimeFilter &upMovingFilter) noexcept;

    StaircaseLooper(const StaircaseLooper &) = delete;
    StaircaseLooper(StaircaseLooper &&) noexcept = delete;
    StaircaseLooper &operator=(const StaircaseLooper &) = delete;
    StaircaseLooper &operator=(StaircaseLooper &&) noexcept = delete;

    ~StaircaseLooper() = default;

    void update(hal::Milliseconds delta) noexcept final;
    std::lock_guard<std::mutex> block() noexcept final;

  private:
    void updateLights(hal::Milliseconds delta) noexcept;
    void updateSensors(hal::Milliseconds delta) noexcept;
    void updateMovigns(hal::Milliseconds delta) noexcept;

    void removeAllStaleMovings(Movings &movings) noexcept;

    void handleDownSensorStateChanged() noexcept;
    void handleUpSensorStateChanged() noexcept;

    bool isFirstMovingFinishing(Movings &movings) const noexcept;
    bool hasNewMovingJustStarted(Movings &movings) const noexcept;
    bool isMoreNewMovingsAvailable(Movings &movings) const noexcept;

    void finishFirstMoving(Movings &movings,
                           IMovingTimeFilter &filter) noexcept;

    BasicLights &mLights;
    IProximitySensor &mDownSensor;
    IProximitySensor &mUpSensor;
    IMovingFactory &mMovingFactory;
    IMovingDurationCalculator &mDurationCalculator;
    IMovingTimeFilter &mDownMovingFilter;
    IMovingTimeFilter &mUpMovingFilter;
    Movings mDownMovings;
    Movings mUpMovings;

    std::mutex mLock;
};

} // namespace staircase