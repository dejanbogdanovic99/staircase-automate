#pragma once

#include <hal/Timing.hxx>

#include <deque>
#include <functional>
#include <memory>

namespace staircase {

class IMoving {
  public:
    enum class Direction { UP, DOWN };

    static constexpr std::size_t kMaxMovings = MAX_MOVINGS;

    virtual ~IMoving() = default;
    virtual void update(hal::Milliseconds delta) noexcept = 0;
    virtual hal::Milliseconds getTimePassed() const noexcept = 0;
    virtual bool isCompleted() const noexcept = 0;
    virtual bool isNearEnd() const noexcept = 0;
    virtual bool isNearBegin() const noexcept = 0;
    virtual bool isTooOld() const noexcept = 0;
};

using MovingPtr = std::unique_ptr<IMoving, std::function<void(IMoving *)>>;
using Movings = std::deque<MovingPtr>;

} // namespace staircase