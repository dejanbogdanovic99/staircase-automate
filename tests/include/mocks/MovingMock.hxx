#include <gmock/gmock.h>

#include <hal/Timing.hxx>

#include <staircase/IMoving.hxx>

namespace tests {
namespace mocks {

class MovingMock : public staircase::IMoving {
  public:
    MOCK_METHOD(void, update, (hal::Milliseconds), (noexcept));
    MOCK_METHOD(hal::Milliseconds, getTimePassed, (), (const, noexcept));
    MOCK_METHOD(bool, isCompleted, (), (const, noexcept));
    MOCK_METHOD(bool, isNearEnd, (), (const, noexcept));
    MOCK_METHOD(bool, isNearBegin, (), (const, noexcept));
    MOCK_METHOD(bool, isTooOld, (), (const, noexcept));
    MOCK_METHOD(void, complete, (), (noexcept));
};

} // namespace mocks
} // namespace tests