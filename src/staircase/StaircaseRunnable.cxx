#include <staircase/StaircaseRunnable.hxx>

#include <hal/ITask.hxx>
#include <hal/Timing.hxx>

#include <staircase/IStaircaseLooper.hxx>

using namespace staircase;

StaircaseRunnable::StaircaseRunnable(hal::ITask &task, IStaircaseLooper &looper)
    : mStaircaseLooper{looper}, mTask{task} {}

void StaircaseRunnable::run() noexcept {
    mStaircaseLooper.update(mTask.getDelta());
}