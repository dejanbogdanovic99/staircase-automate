#pragma once

#include <hal/Timing.hxx>

namespace hal {

class ITiming {
  public:
    ITiming() = default;

    ITiming(const ITiming &) = delete;
    ITiming(ITiming &&) = delete;
    ITiming &operator=(const ITiming &) = delete;
    ITiming &operator=(ITiming &&) = delete;

    virtual ~ITiming() = default;

    virtual void sleepFor(Milliseconds millis) const noexcept = 0;

    virtual Milliseconds getDelta() const noexcept = 0;
};

} // namespace hal