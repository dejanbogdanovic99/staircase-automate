#pragma once

#include <hal/Timing.hxx>

namespace hal {

class INTP {
  public:
    virtual ~INTP() = default;

    hal::Milliseconds obtainCurrentTimeOfDay() noexcept;
};

} // namespace hal