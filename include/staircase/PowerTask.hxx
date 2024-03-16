#pragma once

#include <hal/IPowerManager.hxx>
#include <hal/IRTC.hxx>
#include <hal/Task.hxx>
#include <hal/Timing.hxx>

namespace staircase {

class PowerTask : public hal::Task {
  public:
    PowerTask(hal::IRTC &rtc, hal::IPowerManager &powerManager);

  private:
    static constexpr hal::Milliseconds kUpdateInterval = 1000;

    void run() noexcept final;

    hal::IRTC &mRtc;
    hal::IPowerManager &mPowerManager;
};

} // namespace staircase