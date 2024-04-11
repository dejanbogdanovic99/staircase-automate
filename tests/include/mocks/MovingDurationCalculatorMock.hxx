#include <gmock/gmock.h>

#include <hal/Timing.hxx>

#include <staircase/IMovingDurationCalculator.hxx>

#include <cstdint>

namespace tests {
namespace mocks {

class MovingDurationCalculatorMock
    : public staircase::IMovingDurationCalculator {
  public:
    MOCK_METHOD(hal::Milliseconds, calculateDelta,
                (std::size_t, hal::Milliseconds), (const, noexcept));
};

} // namespace mocks
} // namespace tests