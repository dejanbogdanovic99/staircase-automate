#pragma once

#include <hal/Timing.hxx>

#include <staircase/IMovingTimeFilter.hxx>

#include <array>
#include <cstdint>

namespace staircase {
class MTAMovingTimeFilter final : public IMovingTimeFilter {
  public:
    MTAMovingTimeFilter(hal::Milliseconds initialFilterValue);

    hal::Milliseconds getCurrentMovingTime() const noexcept override;
    void processNewMovingTime(hal::Milliseconds timeElapsed) noexcept override;
    void reset(hal::Milliseconds timeElapsed) noexcept override;

  private:
    static constexpr std::size_t kMTASize = 5;

    std::array<hal::Milliseconds, kMTASize> mFilterValues;
    std::size_t mCurrentIndex;
    hal::Milliseconds mCurrentMovingTime;
};
} // namespace staircase