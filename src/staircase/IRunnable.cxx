#include <staircase/IRunnable.hxx>

#include <hal/ITask.hxx>

using namespace staircase;

IRunnable::IRunnable() : mTask{nullptr} {}

void IRunnable::setParentTask(hal::ITask *task) noexcept { mTask = task; }