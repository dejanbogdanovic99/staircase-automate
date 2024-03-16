#include <staircase/NTPRunnable.hxx>

#include <hal/INTP.hxx>
#include <hal/IRTC.hxx>
#include <hal/Timing.hxx>

using namespace staircase;

NTPRunnable::NTPRunnable(hal::IRTC &rtc, hal::INTP &ntp)
    : mRtc{rtc}, mNtp{ntp} {}

void NTPRunnable::run() noexcept {
    hal::Milliseconds ntpTime = mNtp.obtainCurrentTimeOfDay();
    hal::Milliseconds rtcTime = mRtc.getCurrentTimeOfDay();
    mRtc.adjustCurrentTime(ntpTime - rtcTime);
}