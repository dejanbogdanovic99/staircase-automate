#include <hal/Task.hxx>

#include <hal/Timing.hxx>

using namespace hal;

Task::Task(Milliseconds period)
    : mPeriod{period}, mRunning{true} {}

void Task::loop() noexcept {
    while (mRunning.load()) {
        run();
        sleep();
    }
}