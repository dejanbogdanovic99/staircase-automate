#pragma once

#include <hal/Timing.hxx>

#include <util/StaticDequeue.hxx>

namespace staircase {

class IMoving {
  public:
    enum class Direction { UP, DOWN };

    virtual void update(hal::Milliseconds delta) noexcept = 0;
    virtual hal::Milliseconds getTimePassed() const noexcept = 0;
    virtual bool isCompleted() const noexcept = 0;
    virtual bool isNearEnd() const noexcept = 0;
    virtual bool isNearBegin() const noexcept = 0;
    virtual bool isTooOld() const noexcept = 0;
    virtual void complete() noexcept = 0;
};

static constexpr std::size_t kMaxMovings = 3;
using Movings = util::StaticDequeue<IMoving *, kMaxMovings>;

} // namespace staircase