#include <gmock/gmock.h>

#include <staircase/IBasicLight.hxx>

namespace tests {
namespace mocks {

class BasicLightMock : public staircase::IBasicLight {
  public:
    MOCK_METHOD(void, turnOn, (hal::Milliseconds), (noexcept));
    MOCK_METHOD(void, turnOff, (hal::Milliseconds), (noexcept));
    MOCK_METHOD(void, update, (hal::Milliseconds), (noexcept));
};

} // namespace mocks
} // namespace tests