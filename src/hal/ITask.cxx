#include <hal/ITask.hxx>

#include <hal/Timing.hxx>
#include <staircase/IRunnable.hxx>

using namespace hal;

ITask::ITask(staircase::IRunnable &runnable, hal::Milliseconds period)
    : mPeriod{period}, mRunnable{runnable}, mRunning{true} {
    mRunnable.setParentTask(this);
}

void ITask::loop() noexcept {
    while (mRunning.load()) {
        mRunnable.run();
        sleep();
    }
}