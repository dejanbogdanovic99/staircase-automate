#include <staircase/ClippedSquaredMovingDurationCalculator.hxx>

#include <hal/Timing.hxx>

#include <staircase/IBasicLight.hxx>

#include <cstdint>

using namespace staircase;

hal::Milliseconds ClippedSquaredMovingDurationCalculator::calculateDelta(
    std::size_t lightIndex, hal::Milliseconds totalDuration) const noexcept {

    switch (lightIndex) {
    case 0:
        return kFirstLightDelta;
    case 1:
        return kSecondLightDelta;
    case 2:
        return kThirdLightDelta;
    default:
        return totalDuration / IBasicLight::kLightsNum;
    }
}