#pragma once

#include <hal/Timing.hxx>

#include <staircase/IBasicLight.hxx>
#include <staircase/IMoving.hxx>

#include <cstdint>

namespace staircase {

class Moving final : public IMoving {
  public:
    Moving(BasicLights lights, Direction direction,
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
    void complete() noexcept final;

  private:
    static constexpr hal::Milliseconds kCloseFinishDiff = 2000;
    static constexpr hal::Milliseconds kFastRiseTime = 500;
    static constexpr std::size_t kFastRiseCount = 3;
    hal::Milliseconds calculateInterval() const noexcept;

    using BasicLightIterator = BasicLights::iterator;

    BasicLights mLights;
    BasicLightIterator mCurrentLight;
    std::size_t mCurrentIndex;
    bool mCompleted;
    Direction mDirection;
    hal::Milliseconds mDuration;
    hal::Milliseconds mStandardOnDuration;
    hal::Milliseconds mTimeLeftUntillUpdate;
    hal::Milliseconds mTimePassed;
};

} // namespace staircase