#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <staircase/ClippedSquaredMovingDurationCalculator.hxx>
#include <staircase/IBasicLight.hxx>

#include <algorithm>
#include <array>

namespace tests {

using ::testing::_;

TEST(ClippedSquaredMovingDurationCalculatorTests,
     GivenCalculateDeltaIsCalledWithIndex0ItReturns500) {
    staircase::ClippedSquaredMovingDurationCalculator calculator;
    EXPECT_EQ(calculator.calculateDelta(0, 8000), 500);
}

TEST(ClippedSquaredMovingDurationCalculatorTests,
     GivenCalculateDeltaIsCalledWithIndex1ItReturns850) {
    staircase::ClippedSquaredMovingDurationCalculator calculator;
    EXPECT_EQ(calculator.calculateDelta(1, 8000), 850);
}

TEST(ClippedSquaredMovingDurationCalculatorTests,
     GivenCalculateDeltaIsCalledWithIndex2ItReturns1500) {
    staircase::ClippedSquaredMovingDurationCalculator calculator;
    EXPECT_EQ(calculator.calculateDelta(2, 8000), 1500);
}

TEST(ClippedSquaredMovingDurationCalculatorTests,
     GivenCalculateDeltaIsCalledWithOtherIndexesItReturnsPercentage) {
    staircase::ClippedSquaredMovingDurationCalculator calculator;
    constexpr hal::Milliseconds delta =
        8000 / staircase::IBasicLight::kLightsNum;
    EXPECT_EQ(calculator.calculateDelta(3, 8000), delta);
    EXPECT_EQ(calculator.calculateDelta(4, 8000), delta);
    EXPECT_EQ(calculator.calculateDelta(5, 8000), delta);
    EXPECT_EQ(calculator.calculateDelta(6, 8000), delta);
    EXPECT_EQ(calculator.calculateDelta(7, 8000), delta);
}

} // namespace tests