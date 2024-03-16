#pragma once

#include <hal/Task.hxx>
#include <hal/Timing.hxx>

#include <staircase/IStaircaseLooper.hxx>

namespace staircase {

class StaircaseHandlingTask : public hal::Task {
  public:
    StaircaseHandlingTask(IStaircaseLooper &looper, hal::Milliseconds period);

  private:
    void run() noexcept final;

    IStaircaseLooper &mStaircaseLooper;
};
} // namespace staircase