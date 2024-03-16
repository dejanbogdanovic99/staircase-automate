#pragma once

#include <hal/ITask.hxx>
#include <hal/Timing.hxx>

#include <staircase/IRunnable.hxx>
#include <staircase/IStaircaseLooper.hxx>

namespace staircase {

class StaircaseRunnable : public IRunnable {
  public:
    static constexpr hal::Milliseconds kUpdateInterval = 10;

    StaircaseRunnable(IStaircaseLooper &looper);

  private:
    void run() noexcept final;

    IStaircaseLooper &mStaircaseLooper;
};
} // namespace staircase