#include <staircase/StaircaseHandlingTask.hxx>

#include <hal/Task.hxx>
#include <hal/Timing.hxx>

#include <staircase/IStaircaseLooper.hxx>

using namespace staircase;

StaircaseHandlingTask::StaircaseHandlingTask(IStaircaseLooper &looper,
                                             hal::Milliseconds period)
    : Task{period}, mStaircaseLooper{looper} {}

void StaircaseHandlingTask::run() noexcept {
    mStaircaseLooper.update(getDelta());
}