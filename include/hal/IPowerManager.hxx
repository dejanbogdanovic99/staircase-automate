#pragma once

#include <hal/Timing.hxx>

namespace hal {

class IPowerManager {
  public:
    virtual ~IPowerManager() = default;
    virtual void hibernateFor(hal::Milliseconds milliseconds) noexcept = 0;
};

} // namespace hal