#pragma once

#include <hal/Timing.hxx>

#include <staircase/IBasicLight.hxx>
#include <staircase/IMoving.hxx>

#include <cstdint>

namespace staircase {

class Moving : public IMoving {
  public:
    Moving(BasicLights &lights, Direction direction,
           hal::Milliseconds duration) noexcept;

    Moving(const Moving &) = delete;
    Moving(Moving &&) noexcept = default;
    Moving &operator=(const Moving &) = delete;
    Moving &operator=(Moving &&) noexcept = default;

    ~Moving() = default;

    void update(hal::Milliseconds delta) noexcept final;
    hal::Milliseconds getTimePassed() const noexcept final;
    bool isCompleted() const noexcept final;
    bool isNearEnd() const noexcept final;
    bool isNearBegin() const noexcept final;
    bool isTooOld() const noexcept final;

  private:
    void turnCurrentOn() noexcept;
    hal::Milliseconds calculateInterval() const noexcept;

    static constexpr hal::Milliseconds kCloseFinishDiff = MOVING_FINISH_DELTA;
    static constexpr hal::Milliseconds kFastRiseTime = 500;
    static constexpr std::size_t kFastRiseCount = 3;

    BasicLights &mLights;
    std::size_t mCurrentIndex;
    bool mCompleted;
    Direction mDirection;
    hal::Milliseconds mExpectedDuration;
    hal::Milliseconds mTimeLeftUntilUpdate;
    hal::Milliseconds mTimePassed;
};

} // namespace staircase