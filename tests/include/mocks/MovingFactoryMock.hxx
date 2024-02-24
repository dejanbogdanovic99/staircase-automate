#include <gmock/gmock.h>

#include <hal/Timing.hxx>

#include <staircase/IBasicLight.hxx>
#include <staircase/IMoving.hxx>
#include <staircase/IMovingFactory.hxx>

namespace tests {
namespace mocks {

class MovingFactoryMock : public staircase::IMovingFactory {
  public:
    MOCK_METHOD(staircase::IMoving *, create,
                (staircase::BasicLights&, staircase::IMoving::Direction,
                 hal::Milliseconds),
                (noexcept));
    MOCK_METHOD(void, destroy, (staircase::IMoving * moving), (noexcept));
};

} // namespace mocks
} // namespace tests