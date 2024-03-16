#pragma once

namespace hal {
class ITask;
}

namespace staircase {

class IRunnable {
  public:
    IRunnable();
    virtual ~IRunnable() = default;

    virtual void run() noexcept = 0;
    void setParentTask(hal::ITask *task) noexcept;

  protected:
    hal::ITask *mTask;
};

} // namespace staircase