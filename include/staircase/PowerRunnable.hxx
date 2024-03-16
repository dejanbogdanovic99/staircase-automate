#pragma once

#include <hal/IPowerManager.hxx>
#include <hal/IRTC.hxx>
#include <hal/Timing.hxx>

#include <staircase/IRunnable.hxx>

namespace staircase {

class PowerRunnable : public IRunnable {
  public:
    static constexpr hal::Milliseconds kUpdateInterval = 1000;
    PowerRunnable(hal::IRTC &rtc, hal::IPowerManager &powerManager);

  private:
    void run() noexcept final;

    hal::IRTC &mRtc;
    hal::IPowerManager &mPowerManager;
};

} // namespace staircase