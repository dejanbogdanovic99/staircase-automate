#pragma once

#include <hal/Timing.hxx>

#include <chrono>
#include <cstdint>

namespace hal {

class IRTC {
  public:
    virtual ~IRTC() = default;
    virtual hal::Milliseconds getCurrentTimeOfDay() const noexcept = 0;
    virtual void adjustCurrentTime(hal::Milliseconds diff) noexcept = 0;
};

} // namespace hal