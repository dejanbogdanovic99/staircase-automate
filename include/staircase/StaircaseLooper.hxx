#pragma once

#include <hal/Timing.hxx>

#include <staircase/IBasicLight.hxx>
#include <staircase/IMoving.hxx>
#include <staircase/IMovingFactory.hxx>
#include <staircase/IProximitySensor.hxx>
#include <staircase/IStaircaseLooper.hxx>

#include <util/StaticDequeue.hxx>

#include <cstdint>

namespace staircase {

class StaircaseLooper final : public IStaircaseLooper {
  public:
    StaircaseLooper(BasicLights &lights, IProximitySensor &downSensor,
                    IProximitySensor &upSensor,
                    IMovingFactory &movingFactory) noexcept;

    StaircaseLooper(const StaircaseLooper &) = delete;
    StaircaseLooper(StaircaseLooper &&) noexcept = delete;
    StaircaseLooper &operator=(const StaircaseLooper &) = delete;
    StaircaseLooper &operator=(StaircaseLooper &&) noexcept = delete;

    ~StaircaseLooper() = default;

    [[noreturn]] void run(hal::ITiming &timing,
                          hal::Milliseconds period) noexcept final;
    void update(hal::Milliseconds delta) noexcept final;

  private:
    static constexpr std::size_t kMovingsAllowed = 3;
    static constexpr hal::Milliseconds kInitialDownMovingDuration = 12000;
    static constexpr hal::Milliseconds kInitialUpMovingDuration = 12000;

    void updateLights(hal::Milliseconds delta) noexcept;
    void updateSensors(hal::Milliseconds delta) noexcept;
    void updateMovigns(hal::Milliseconds delta) noexcept;

    void removeAllStaleMovings(Movings &movings) noexcept;

    void handleDownSensorStateChanged() noexcept;
    void handleUpSensorStateChanged() noexcept;

    bool isFirstMovingFinishing(Movings &movings) const noexcept;
    bool hasNewMovingJustStarted(Movings &movings) const noexcept;

    void finishFirstMoving(Movings &movings,
                           hal::Milliseconds &duration) noexcept;

    BasicLights &mLights;
    IProximitySensor &mDownSensor;
    IProximitySensor &mUpSensor;
    IMovingFactory &mMovingFactory;
    hal::Milliseconds mDownMovingDuration;
    hal::Milliseconds mUpMovingDuration;
    Movings mDownMovings;
    Movings mUpMovings;
};

} // namespace staircase