#include <gmock/gmock.h>

#include <hal/ITiming.hxx>
#include <hal/Timing.hxx>

namespace tests {
namespace mocks {

class TimingMock : public hal::ITiming {
  public:
    MOCK_METHOD(void, sleepFor, (hal::Milliseconds), (const noexcept));
    MOCK_METHOD(hal::Milliseconds, getDelta, (), (const noexcept));
};

} // namespace mocks
} // namespace tests