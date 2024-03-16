#pragma once

#include <hal/Timing.hxx>

#include <atomic>

namespace hal {

class Task {
  public:
    Task(Milliseconds period);
    virtual ~Task() = default;
    void suspend() noexcept;
    void resume() noexcept;
    void start() noexcept;
    void stop() noexcept;

  protected:
    virtual void run() noexcept = 0;
    virtual Milliseconds getDelta() const noexcept;

  private:
    void loop() noexcept;
    virtual void sleep() const noexcept;

    Milliseconds mPeriod;
    std::atomic_bool mRunning;
};

} // namespace hal