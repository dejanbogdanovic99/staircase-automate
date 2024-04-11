#include <gmock/gmock.h>

#include <hal/Timing.hxx>

#include <staircase/IMovingTimeFilter.hxx>

namespace tests {
namespace mocks {

class MovingTimeFilterMock : public staircase::IMovingTimeFilter {
  public:
    MOCK_METHOD(hal::Milliseconds, getCurrentMovingTime, (), (const, noexcept));
    MOCK_METHOD(void, processNewMovingTime, (hal::Milliseconds), (noexcept));
    MOCK_METHOD(void, reset, (hal::Milliseconds), (noexcept));
};

} // namespace mocks
} // namespace tests