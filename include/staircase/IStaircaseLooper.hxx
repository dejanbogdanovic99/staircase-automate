#pragma once

#include <hal/Timing.hxx>

#include <mutex>

namespace staircase {

class IStaircaseLooper {
  public:
    virtual ~IStaircaseLooper() = default;
    virtual void update(hal::Milliseconds delta) noexcept = 0;
    virtual std::lock_guard<std::mutex> block() noexcept = 0;
};

} // namespace staircase