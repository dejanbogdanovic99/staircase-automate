#include <staircase/PowerTask.hxx>

#include <hal/IPowerManager.hxx>
#include <hal/IRTC.hxx>
#include <hal/Timing.hxx>

#include <cstdint>

using namespace staircase;

PowerTask::PowerTask(hal::IRTC &rtc, hal::IPowerManager &powerManager)
    : Task{kUpdateInterval}, mRtc{rtc}, mPowerManager{powerManager} {}

void PowerTask::run() noexcept {
    hal::Milliseconds currentTime = mRtc.getCurrentTimeOfDay();

    constexpr hal::Milliseconds startSleepTime = 8 * 60 * 60 * 1000;
    constexpr hal::Milliseconds endSleepTime = 17 * 60 * 60 * 1000;

    if ((currentTime > startSleepTime) && (currentTime < endSleepTime)) {
        mPowerManager.hibernateFor(
            static_cast<uint64_t>(endSleepTime - currentTime) * 1000);
    }
}