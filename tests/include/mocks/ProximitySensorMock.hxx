#include <gmock/gmock.h>

#include <hal/Timing.hxx>

#include <staircase/IProximitySensor.hxx>

namespace tests {
namespace mocks {

class ProximitySensorMock : public staircase::IProximitySensor {
  public:
    MOCK_METHOD(bool, stateChanged, (), (const, noexcept));
    MOCK_METHOD(bool, isClose, (), (const, noexcept));
    MOCK_METHOD(bool, isFar, (), (const, noexcept));
    MOCK_METHOD(void, update, (hal::Milliseconds), (noexcept));
};

} // namespace mocks
} // namespace tests