#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <mocks/BasicLightMock.hxx>
#include <mocks/MovingDurationCalculatorMock.hxx>

#include <hal/Timing.hxx>

#include <staircase/IBasicLight.hxx>
#include <staircase/Moving.hxx>

#include <algorithm>
#include <array>

namespace tests {

using ::testing::_;
using ::testing::Exactly;
using ::testing::NiceMock;
using ::testing::Return;

class MovingInitializationTests : public ::testing::Test {
  public:
    MovingInitializationTests()
        : mBasicLightRefs{mBasicLights[0], mBasicLights[1], mBasicLights[2],
                          mBasicLights[3], mBasicLights[4], mBasicLights[5],
                          mBasicLights[6], mBasicLights[7]} {
        ON_CALL(mDurationCalculator, calculateDelta(_, _))
            .WillByDefault(Return(100));
    }

  protected:
    std::array<NiceMock<mocks::BasicLightMock>,
               staircase::IBasicLight::kLightsNum>
        mBasicLights;
    NiceMock<mocks::MovingDurationCalculatorMock> mDurationCalculator;
    staircase::BasicLights mBasicLightRefs;
};

TEST_F(MovingInitializationTests,
       GivenUpMovingIsInitializedFirstLedIsTurnedOn) {
    EXPECT_CALL(mBasicLights[0],
                turnOn(staircase::IBasicLight::kDefaultOnPeriod))
        .Times(Exactly(1));
    staircase::Moving moving{mBasicLightRefs, mDurationCalculator,
                             staircase::Moving::Direction::UP, 12000};
}

TEST_F(MovingInitializationTests,
       GivenDownMovingIsInitializedLastLedIsTurnedOn) {
    EXPECT_CALL(mBasicLights[staircase::IBasicLight::kLightsNum - 1],
                turnOn(staircase::IBasicLight::kDefaultOnPeriod))
        .Times(Exactly(1));
    staircase::Moving moving{mBasicLightRefs, mDurationCalculator,
                             staircase::Moving::Direction::DOWN, 12000};
}

TEST_F(MovingInitializationTests,
       GivenDownMovingIsInitializedCalculateDeltaIsCalledForTheFirstTime) {
    EXPECT_CALL(mDurationCalculator, calculateDelta(0, 12000))
        .Times(Exactly(1));
    staircase::Moving moving{mBasicLightRefs, mDurationCalculator,
                             staircase::Moving::Direction::DOWN, 12000};
}

class MovingTimeTests : public MovingInitializationTests {
  public:
    MovingTimeTests()
        : MovingInitializationTests{},
          mMoving{mBasicLightRefs, mDurationCalculator,
                  staircase::Moving::Direction::UP, 12000} {}

  protected:
    staircase::Moving mMoving;
};

TEST_F(MovingTimeTests,
       GivenUpdateIsCalledGetTimePassedReturnsAccumulatedTime) {
    mMoving.update(300);
    EXPECT_EQ(mMoving.getTimePassed(), 300);
    mMoving.update(400);
    EXPECT_EQ(mMoving.getTimePassed(), 700);
    mMoving.update(400);
    EXPECT_EQ(mMoving.getTimePassed(), 1100);
    mMoving.update(500);
    EXPECT_EQ(mMoving.getTimePassed(), 1600);
}

TEST_F(MovingTimeTests,
       GivenAccumulatedTimeIsLessThanCloseFinishDiffMovingIsNearBegin) {
    EXPECT_TRUE(mMoving.isNearBegin());
    mMoving.update(300);
    EXPECT_TRUE(mMoving.isNearBegin());
    mMoving.update(400);
    EXPECT_TRUE(mMoving.isNearBegin());
    mMoving.update(400);
    EXPECT_TRUE(mMoving.isNearBegin());
    mMoving.update(500);
    EXPECT_TRUE(mMoving.isNearBegin());
}

TEST_F(MovingTimeTests,
       GivenAccumulatedTimeIsGreaterThanCloseFinishDiffMovingIsNotNearBegin) {
    mMoving.update(2300);
    EXPECT_FALSE(mMoving.isNearBegin());
    mMoving.update(300);
    EXPECT_FALSE(mMoving.isNearBegin());
    mMoving.update(300);
    EXPECT_FALSE(mMoving.isNearBegin());
}

TEST_F(
    MovingTimeTests,
    GivenAccumulatedTimeIsLessThanExpectedTimeMinusCloseFinishDiffMovingIsNotNearEnd) {
    mMoving.update(2300);
    EXPECT_FALSE(mMoving.isNearEnd());
    mMoving.update(300);
    EXPECT_FALSE(mMoving.isNearEnd());
    mMoving.update(300);
    EXPECT_FALSE(mMoving.isNearEnd());
}

TEST_F(MovingTimeTests,
       GivenAccumulatedTimeIsCloseToExpectedTimeMovingIsNearEnd) {
    mMoving.update(10010);
    EXPECT_TRUE(mMoving.isNearEnd());
    mMoving.update(2000);
    EXPECT_TRUE(mMoving.isNearEnd());
    mMoving.update(1500);
    EXPECT_TRUE(mMoving.isNearEnd());
}

TEST_F(
    MovingTimeTests,
    GivenAccumulatedTimeIsMoreThanExpectedTimePlusCloseFinishDiffMovingIsNotNearEnd) {
    mMoving.update(14001);
    EXPECT_FALSE(mMoving.isNearEnd());
    mMoving.update(300);
    EXPECT_FALSE(mMoving.isNearEnd());
    mMoving.update(300);
    EXPECT_FALSE(mMoving.isNearEnd());
}

TEST_F(
    MovingTimeTests,
    GivenAccumulatedTimeIsLessThanExpectedTimePlusCloseFinishDiffMovingIsNotTooOld) {
    EXPECT_FALSE(mMoving.isTooOld());
    mMoving.update(3500);
    EXPECT_FALSE(mMoving.isTooOld());
    mMoving.update(3500);
    EXPECT_FALSE(mMoving.isTooOld());
    mMoving.update(3500);
    EXPECT_FALSE(mMoving.isTooOld());
    mMoving.update(3000);
}

TEST_F(
    MovingTimeTests,
    GivenAccumulatedTimeIsMoreThanExpectedTimePlusCloseFinishDiffMovingIsTooOld) {
    mMoving.update(14500);
    EXPECT_TRUE(mMoving.isTooOld());
}

TEST_F(MovingTimeTests, GivenUpMovingIsNotFinishedCompletedReturnsFalse) {
    mMoving.update(50);
    EXPECT_FALSE(mMoving.isCompleted());
    mMoving.update(500);
    EXPECT_FALSE(mMoving.isCompleted());
}

TEST_F(MovingTimeTests, GivenUpMovingIsFinishedCompletedReturnsTrue) {
    mMoving.update(12000);
    EXPECT_TRUE(mMoving.isCompleted());
}

class MovingUpLightTests : public MovingInitializationTests {
  public:
    MovingUpLightTests()
        : MovingInitializationTests{},
          mMoving{mBasicLightRefs, mDurationCalculator,
                  staircase::Moving::Direction::UP, 12000} {}

  protected:
    staircase::Moving mMoving;
};

TEST_F(MovingUpLightTests,
       GivenNewUpMovingIsCreatedItCorrectlyTurningOnLights) {
    mMoving.update(50);

    for (std::size_t i = 1; i < mBasicLights.size(); ++i) {
        EXPECT_CALL(mBasicLights[i],
                    turnOn(staircase::IBasicLight::kDefaultOnPeriod))
            .Times(Exactly(1));
        EXPECT_CALL(mDurationCalculator, calculateDelta(i, 12000))
            .Times(Exactly(1));
        mMoving.update(100);
    }
}

class MovingDownLightTests : public MovingInitializationTests {
  public:
    MovingDownLightTests()
        : MovingInitializationTests{},
          mMoving{mBasicLightRefs, mDurationCalculator,
                  staircase::Moving::Direction::DOWN, 12000} {}

  protected:
    staircase::Moving mMoving;
};

TEST_F(MovingDownLightTests,
       GivenNewUpMovingIsCreatedItCorrectlyTurningOnLights) {
    mMoving.update(50);
    for (std::size_t i = 1; i < mBasicLights.size(); ++i) {
        EXPECT_CALL(mBasicLights[staircase::IBasicLight::kLightsNum - i - 1],
                    turnOn(staircase::IBasicLight::kDefaultOnPeriod))
            .Times(Exactly(1));
        EXPECT_CALL(mDurationCalculator, calculateDelta(i, 12000))
            .Times(Exactly(1));
        mMoving.update(100);
    }
}

} // namespace tests