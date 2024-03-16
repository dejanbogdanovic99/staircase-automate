#pragma once

#include <hal/Timing.hxx>

#include <staircase/IRunnable.hxx>

#include <atomic>

namespace hal {

class ITask {
  public:
    ITask(staircase::IRunnable &runnable, hal::Milliseconds period);
    virtual ~ITask() = default;

    virtual Milliseconds getDelta() const noexcept = 0;

  protected:
    virtual void sleep() noexcept = 0;
    hal::Milliseconds mPeriod;

  private:
    void loop() noexcept;

    staircase::IRunnable &mRunnable;
    std::atomic_bool mRunning;
};

} // namespace hal