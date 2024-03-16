#pragma once

#include <hal/INTP.hxx>
#include <hal/IRTC.hxx>
#include <hal/Timing.hxx>

#include <staircase/IRunnable.hxx>

namespace staircase {

class NTPRunnable : public IRunnable {
  public:
    static constexpr hal::Milliseconds kUpdateInterval = 60 * 1000;
    NTPRunnable(hal::IRTC &rtc, hal::INTP &ntp);

  private:
    void run() noexcept final;

    hal::IRTC &mRtc;
    hal::INTP &mNtp;
};

} // namespace staircase