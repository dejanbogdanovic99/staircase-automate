#pragma once

#include <hal/ITiming.hxx>
#include <hal/Timing.hxx>

namespace staircase {

class IStaircaseLooper {
  public:
    virtual ~IStaircaseLooper() = default;
    [[noreturn]] virtual void run(hal::ITiming &timing,
                                  hal::Milliseconds period) noexcept = 0;
    virtual void update(hal::Milliseconds delta) noexcept = 0;
};

} // namespace staircase