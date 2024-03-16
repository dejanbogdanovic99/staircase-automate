#pragma once

#include <hal/ITask.hxx>
#include <hal/Timing.hxx>

#include <staircase/IRunnable.hxx>
#include <staircase/IStaircaseLooper.hxx>

namespace staircase {

class StaircaseRunnable : public IRunnable {
  public:
    static constexpr hal::Milliseconds kUpdateInterval = 15 * 1000;

    StaircaseRunnable(hal::ITask &task, IStaircaseLooper &looper);

  private:
    void run() noexcept final;

    IStaircaseLooper &mStaircaseLooper;
    hal::ITask &mTask;
};
} // namespace staircase