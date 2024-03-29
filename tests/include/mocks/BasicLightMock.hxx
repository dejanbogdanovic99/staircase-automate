#include <gmock/gmock.h>

#include <hal/Timing.hxx>

#include <staircase/IBasicLight.hxx>

namespace tests {
namespace mocks {

class BasicLightMock : public staircase::IBasicLight {
  public:
    MOCK_METHOD(void, turnOn, (hal::Milliseconds), (noexcept));
    MOCK_METHOD(void, turnOff, (), (noexcept));
    MOCK_METHOD(void, update, (hal::Milliseconds), (noexcept));
    MOCK_METHOD(bool, isOn, (), (const, noexcept));
    MOCK_METHOD(bool, isOff, (), (const, noexcept));
};

} // namespace mocks
} // namespace tests