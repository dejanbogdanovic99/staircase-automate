#include <staircase/StaircaseRunnable.hxx>

#include <hal/ITask.hxx>
#include <hal/Timing.hxx>

#include <staircase/IStaircaseLooper.hxx>

using namespace staircase;

StaircaseRunnable::StaircaseRunnable(IStaircaseLooper &looper)
    : mStaircaseLooper{looper} {}

void StaircaseRunnable::run() noexcept {
    hal::Milliseconds delta = kUpdateInterval;
    if (mTask) {
        delta = mTask->getDelta();
    }
    mStaircaseLooper.update(delta);
}