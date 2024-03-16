#pragma once

#include <hal/INTP.hxx>
#include <hal/IRTC.hxx>
#include <hal/Task.hxx>
#include <hal/Timing.hxx>

namespace staircase {

class NTPTask : public hal::Task {
  public:
    NTPTask(hal::IRTC &rtc, hal::INTP &ntp);

  private:
    static constexpr hal::Milliseconds kUpdateInterval = 10 * 60 * 1000;

    void run() noexcept final;

    hal::IRTC &mRtc;
    hal::INTP &mNtp;
};

} // namespace staircase