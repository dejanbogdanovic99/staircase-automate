#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <mocks/BasicLightMock.hxx>
#include <mocks/MovingFactoryMock.hxx>
#include <mocks/MovingMock.hxx>
#include <mocks/ProximitySensorMock.hxx>

#include <hal/Timing.hxx>

#include <staircase/IBasicLight.hxx>
#include <staircase/IMoving.hxx>
#include <staircase/Moving.hxx>
#include <staircase/StaircaseLooper.hxx>

#include <algorithm>
#include <array>

namespace tests {

using ::testing::_;
using ::testing::Exactly;
using ::testing::InSequence;
using ::testing::NiceMock;
using ::testing::Return;

class StaircaseLooperTests : public ::testing::Test {
  public:
    void SetUp() {
        std::transform(std::begin(mBasicLights), std::end(mBasicLights),
                       std::begin(mBasicLightPointers),
                       [](auto &basicLight) { return &basicLight; });
        ON_CALL(mDownSensor, stateChanged()).WillByDefault(Return(false));
        ON_CALL(mUpSensor, stateChanged()).WillByDefault(Return(false));
    }

    void TearDown() {}

  protected:
    std::array<NiceMock<mocks::BasicLightMock>, staircase::kLightNum>
        mBasicLights;
    std::array<staircase::IBasicLight *, staircase::kLightNum>
        mBasicLightPointers;
    NiceMock<mocks::ProximitySensorMock> mDownSensor;
    NiceMock<mocks::ProximitySensorMock> mUpSensor;
    NiceMock<mocks::MovingFactoryMock> mMovingFactory;
};

TEST_F(StaircaseLooperTests, LightUpdateCallCallChecks) {
    staircase::StaircaseLooper looper{
        staircase::BasicLights{std::begin(mBasicLightPointers),
                               std::end(mBasicLightPointers)},
        mDownSensor, mUpSensor, mMovingFactory};

    constexpr hal::Milliseconds timeInterval = 1001;

    std::for_each(
        std::begin(mBasicLights), std::end(mBasicLights),
        [timeInterval](auto &basicLight) {
            EXPECT_CALL(basicLight, update(timeInterval)).Times(Exactly(1));
        });

    looper.update(timeInterval);
}

TEST_F(StaircaseLooperTests, SensorUpdateCallCallChecks) {
    staircase::StaircaseLooper looper{
        staircase::BasicLights{std::begin(mBasicLightPointers),
                               std::end(mBasicLightPointers)},
        mDownSensor, mUpSensor, mMovingFactory};

    constexpr hal::Milliseconds timeInterval = 1001;

    EXPECT_CALL(mDownSensor, update(timeInterval)).Times(Exactly(1));
    EXPECT_CALL(mUpSensor, update(timeInterval)).Times(Exactly(1));

    looper.update(timeInterval);
}

TEST_F(StaircaseLooperTests, OneUpMovingStarts) {
    staircase::StaircaseLooper looper{
        staircase::BasicLights{std::begin(mBasicLightPointers),
                               std::end(mBasicLightPointers)},
        mDownSensor, mUpSensor, mMovingFactory};

    mocks::MovingMock moving;

    InSequence s;

    EXPECT_CALL(mDownSensor, stateChanged()).WillOnce(Return(true));
    EXPECT_CALL(mDownSensor, isClose()).WillOnce(Return(true));
    EXPECT_CALL(mMovingFactory, create(_, staircase::IMoving::Direction::UP, _))
        .WillOnce(Return(&moving));
    looper.update(100);
}

TEST_F(StaircaseLooperTests, OneUpMovingToStale) {
    staircase::StaircaseLooper looper{
        staircase::BasicLights{std::begin(mBasicLightPointers),
                               std::end(mBasicLightPointers)},
        mDownSensor, mUpSensor, mMovingFactory};

    NiceMock<mocks::MovingMock> moving;

    {
        InSequence s;

        EXPECT_CALL(mDownSensor, stateChanged()).WillOnce(Return(true));
        EXPECT_CALL(mDownSensor, isClose()).WillOnce(Return(true));
        EXPECT_CALL(mMovingFactory,
                    create(_, staircase::IMoving::Direction::UP, _))
            .WillOnce(Return(&moving));
        looper.update(100);
    }

    {
        InSequence s;

        EXPECT_CALL(moving, update(_)).Times(Exactly(1));
        EXPECT_CALL(moving, isTooOld()).WillOnce(Return(true));
        EXPECT_CALL(mMovingFactory, destroy(&moving)).Times(Exactly(1));
        EXPECT_CALL(mDownSensor, stateChanged()).WillOnce(Return(false));
        looper.update(100);
    }

    {
        InSequence s;

        EXPECT_CALL(moving, update(_)).Times(Exactly(0));
        EXPECT_CALL(mDownSensor, stateChanged()).WillOnce(Return(false));
        looper.update(100);
    }
}

TEST_F(StaircaseLooperTests, OneUpMovingEndBySensor) {
    staircase::StaircaseLooper looper{
        staircase::BasicLights{std::begin(mBasicLightPointers),
                               std::end(mBasicLightPointers)},
        mDownSensor, mUpSensor, mMovingFactory};

    NiceMock<mocks::MovingMock> moving;

    {
        InSequence s;

        EXPECT_CALL(mDownSensor, stateChanged()).WillOnce(Return(true));
        EXPECT_CALL(mDownSensor, isClose()).WillOnce(Return(true));
        EXPECT_CALL(mMovingFactory,
                    create(_, staircase::IMoving::Direction::UP, _))
            .WillOnce(Return(&moving));
        looper.update(100);
    }

    {
        InSequence s;

        EXPECT_CALL(moving, update(100)).Times(Exactly(1));
        EXPECT_CALL(mDownSensor, stateChanged()).WillOnce(Return(false));
        EXPECT_CALL(mUpSensor, stateChanged()).WillOnce(Return(true));
        EXPECT_CALL(mUpSensor, isClose()).WillOnce(Return(true));
        EXPECT_CALL(moving, isNearEnd()).WillOnce(Return(true));
        EXPECT_CALL(moving, getTimePassed()).Times(Exactly(1));
        EXPECT_CALL(mMovingFactory, destroy(&moving)).Times(Exactly(1));
        looper.update(100);
    }

    {
        InSequence s;

        EXPECT_CALL(moving, update(_)).Times(Exactly(0));
        EXPECT_CALL(mDownSensor, stateChanged()).WillOnce(Return(false));
        EXPECT_CALL(mUpSensor, stateChanged()).WillOnce(Return(false));
        looper.update(100);
    }
}

TEST_F(StaircaseLooperTests, OneDownMovingStarts) {
    staircase::StaircaseLooper looper{
        staircase::BasicLights{std::begin(mBasicLightPointers),
                               std::end(mBasicLightPointers)},
        mDownSensor, mUpSensor, mMovingFactory};

    mocks::MovingMock moving;

    InSequence s;

    EXPECT_CALL(mUpSensor, stateChanged()).WillOnce(Return(true));
    EXPECT_CALL(mUpSensor, isClose()).WillOnce(Return(true));
    EXPECT_CALL(mMovingFactory,
                create(_, staircase::IMoving::Direction::DOWN, _))
        .WillOnce(Return(&moving));
    looper.update(100);
}

TEST_F(StaircaseLooperTests, OneDownMovingToStale) {
    staircase::StaircaseLooper looper{
        staircase::BasicLights{std::begin(mBasicLightPointers),
                               std::end(mBasicLightPointers)},
        mDownSensor, mUpSensor, mMovingFactory};

    NiceMock<mocks::MovingMock> moving;

    {
        InSequence s;

        EXPECT_CALL(mUpSensor, stateChanged()).WillOnce(Return(true));
        EXPECT_CALL(mUpSensor, isClose()).WillOnce(Return(true));
        EXPECT_CALL(mMovingFactory,
                    create(_, staircase::IMoving::Direction::DOWN, _))
            .WillOnce(Return(&moving));
        looper.update(100);
    }

    {
        InSequence s;

        EXPECT_CALL(moving, update(_)).Times(Exactly(1));
        EXPECT_CALL(moving, isTooOld()).WillOnce(Return(true));
        EXPECT_CALL(mMovingFactory, destroy(&moving)).Times(Exactly(1));
        EXPECT_CALL(mUpSensor, stateChanged()).WillOnce(Return(false));
        looper.update(100);
    }

    {
        InSequence s;

        EXPECT_CALL(moving, update(_)).Times(Exactly(0));
        EXPECT_CALL(mUpSensor, stateChanged()).WillOnce(Return(false));
        looper.update(100);
    }
}

TEST_F(StaircaseLooperTests, OneDownMovingEndBySensor) {
    staircase::StaircaseLooper looper{
        staircase::BasicLights{std::begin(mBasicLightPointers),
                               std::end(mBasicLightPointers)},
        mDownSensor, mUpSensor, mMovingFactory};

    NiceMock<mocks::MovingMock> moving;

    {
        InSequence s;

        EXPECT_CALL(mUpSensor, stateChanged()).WillOnce(Return(true));
        EXPECT_CALL(mUpSensor, isClose()).WillOnce(Return(true));
        EXPECT_CALL(mMovingFactory,
                    create(_, staircase::IMoving::Direction::DOWN, _))
            .WillOnce(Return(&moving));
        looper.update(100);
    }

    {
        InSequence s;

        EXPECT_CALL(moving, update(100)).Times(Exactly(1));
        EXPECT_CALL(mDownSensor, stateChanged()).WillOnce(Return(true));
        EXPECT_CALL(mDownSensor, isClose()).WillOnce(Return(true));
        EXPECT_CALL(moving, isNearEnd()).WillOnce(Return(true));
        EXPECT_CALL(moving, getTimePassed()).Times(Exactly(1));
        EXPECT_CALL(mMovingFactory, destroy(&moving)).Times(Exactly(1));
        EXPECT_CALL(mUpSensor, stateChanged()).WillOnce(Return(false));
        looper.update(100);
    }

    {
        InSequence s;

        EXPECT_CALL(moving, update(_)).Times(Exactly(0));
        EXPECT_CALL(mDownSensor, stateChanged()).WillOnce(Return(false));
        EXPECT_CALL(mUpSensor, stateChanged()).WillOnce(Return(false));
        looper.update(100);
    }
}

} // namespace tests