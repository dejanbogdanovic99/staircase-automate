#include <staircase/NTPTask.hxx>

#include <hal/INTP.hxx>
#include <hal/IRTC.hxx>
#include <hal/Timing.hxx>

using namespace staircase;

NTPTask::NTPTask(hal::IRTC &rtc, hal::INTP &ntp)
    : Task{kUpdateInterval}, mRtc{rtc}, mNtp{ntp} {}

void NTPTask::run() noexcept {
    hal::Milliseconds ntpTime = mNtp.obtainCurrentTimeOfDay();
    hal::Milliseconds rtcTime = mRtc.getCurrentTimeOfDay();
    mRtc.adjustCurrentTime(ntpTime - rtcTime);
}