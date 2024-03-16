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
using ::testing::Invoke;
using ::testing::NiceMock;
using ::testing::Return;

class StaircaseLooperTests : public ::testing::Test {
  public:
    void SetUp() {
        ON_CALL(mDownSensor, hasStateChanged()).WillByDefault(Return(false));
        ON_CALL(mUpSensor, hasStateChanged()).WillByDefault(Return(false));
    }

    void TearDown() {}

    StaircaseLooperTests()
        : mBasicLightRefs{mBasicLights[0], mBasicLights[1], mBasicLights[2],
                          mBasicLights[3], mBasicLights[4], mBasicLights[5],
                          mBasicLights[6], mBasicLights[7]},
          mStaircaseLooper{mBasicLightRefs, mDownSensor, mUpSensor,
                           mMovingFactory} {}

  protected:
    std::array<NiceMock<mocks::BasicLightMock>,
               staircase::IBasicLight::kLightsNum>
        mBasicLights;
    staircase::BasicLights mBasicLightRefs;
    NiceMock<mocks::ProximitySensorMock> mDownSensor;
    NiceMock<mocks::ProximitySensorMock> mUpSensor;
    NiceMock<mocks::MovingFactoryMock> mMovingFactory;
    staircase::StaircaseLooper mStaircaseLooper;
};

TEST_F(StaircaseLooperTests,
       GivenUpdateIsCalledItUpdatesAllTheLightsAndSensors) {
    constexpr hal::Milliseconds timeInterval = 1001;

    std::for_each(
        std::begin(mBasicLights), std::end(mBasicLights),
        [timeInterval](auto &basicLight) {
            EXPECT_CALL(basicLight, update(timeInterval)).Times(Exactly(1));
        });

    EXPECT_CALL(mDownSensor, update(timeInterval));
    EXPECT_CALL(mUpSensor, update(timeInterval));

    mStaircaseLooper.update(timeInterval);
}

TEST_F(StaircaseLooperTests,
       GivenDownSensorStateChangedToCloseNewUpMovingIsCreated) {
    InSequence s;

    EXPECT_CALL(mDownSensor, hasStateChanged()).WillOnce(Return(true));
    EXPECT_CALL(mDownSensor, isClose()).WillOnce(Return(true));
    EXPECT_CALL(mMovingFactory, create(_, staircase::IMoving::Direction::UP, _))
        .WillOnce(Invoke([]() {
            return staircase::MovingPtr{nullptr,
                                        [](staircase::IMoving *moving) {}};
        }));
    mStaircaseLooper.update(100);
}

TEST_F(StaircaseLooperTests, OneUpMovingToStale) {
    staircase::StaircaseLooper looper{mBasicLightRefs, mDownSensor, mUpSensor,
                                      mMovingFactory};

    NiceMock<mocks::MovingMock> moving;

    {
        InSequence s;

        EXPECT_CALL(mDownSensor, hasStateChanged()).WillOnce(Return(true));
        EXPECT_CALL(mDownSensor, isClose()).WillOnce(Return(true));
        EXPECT_CALL(mMovingFactory,
                    create(_, staircase::IMoving::Direction::UP, _))
            .WillOnce(Invoke([&moving]() {
                return staircase::MovingPtr{&moving,
                                            [](staircase::IMoving *moving) {}};
            }));
        looper.update(100);
    }

    {
        InSequence s;

        EXPECT_CALL(moving, update(_)).Times(Exactly(1));
        EXPECT_CALL(moving, isTooOld()).WillOnce(Return(true));
        EXPECT_CALL(mDownSensor, hasStateChanged()).WillOnce(Return(false));
        looper.update(100);
    }

    {
        InSequence s;

        EXPECT_CALL(moving, update(_)).Times(Exactly(0));
        EXPECT_CALL(mDownSensor, hasStateChanged()).WillOnce(Return(false));
        looper.update(100);
    }
}

TEST_F(StaircaseLooperTests, OneUpMovingEndBySensor) {
    staircase::StaircaseLooper looper{mBasicLightRefs, mDownSensor, mUpSensor,
                                      mMovingFactory};

    NiceMock<mocks::MovingMock> moving;

    {
        InSequence s;

        EXPECT_CALL(mDownSensor, hasStateChanged()).WillOnce(Return(true));
        EXPECT_CALL(mDownSensor, isClose()).WillOnce(Return(true));
        EXPECT_CALL(mMovingFactory,
                    create(_, staircase::IMoving::Direction::UP, _))
            .WillOnce(Invoke([&moving]() {
                return staircase::MovingPtr{&moving,
                                            [](staircase::IMoving *moving) {}};
            }));
        looper.update(100);
    }

    {
        InSequence s;

        EXPECT_CALL(moving, update(100)).Times(Exactly(1));
        EXPECT_CALL(mDownSensor, hasStateChanged()).WillOnce(Return(false));
        EXPECT_CALL(mUpSensor, hasStateChanged()).WillOnce(Return(true));
        EXPECT_CALL(mUpSensor, isClose()).WillOnce(Return(true));
        EXPECT_CALL(moving, isNearEnd()).WillOnce(Return(true));
        EXPECT_CALL(moving, getTimePassed()).Times(Exactly(1));
        looper.update(100);
    }

    {
        InSequence s;

        EXPECT_CALL(moving, update(_)).Times(Exactly(0));
        EXPECT_CALL(mDownSensor, hasStateChanged()).WillOnce(Return(false));
        EXPECT_CALL(mUpSensor, hasStateChanged()).WillOnce(Return(false));
        looper.update(100);
    }
}

TEST_F(StaircaseLooperTests, OneDownMovingStarts) {
    staircase::StaircaseLooper looper{mBasicLightRefs, mDownSensor, mUpSensor,
                                      mMovingFactory};

    mocks::MovingMock moving;

    InSequence s;

    EXPECT_CALL(mUpSensor, hasStateChanged()).WillOnce(Return(true));
    EXPECT_CALL(mUpSensor, isClose()).WillOnce(Return(true));
    EXPECT_CALL(mMovingFactory,
                create(_, staircase::IMoving::Direction::DOWN, _))
        .WillOnce(Invoke([&moving]() {
            return staircase::MovingPtr{&moving,
                                        [](staircase::IMoving *moving) {}};
        }));
    looper.update(100);
}

TEST_F(StaircaseLooperTests, OneDownMovingToStale) {
    staircase::StaircaseLooper looper{mBasicLightRefs, mDownSensor, mUpSensor,
                                      mMovingFactory};

    NiceMock<mocks::MovingMock> moving;

    {
        InSequence s;

        EXPECT_CALL(mUpSensor, hasStateChanged()).WillOnce(Return(true));
        EXPECT_CALL(mUpSensor, isClose()).WillOnce(Return(true));
        EXPECT_CALL(mMovingFactory,
                    create(_, staircase::IMoving::Direction::DOWN, _))
            .WillOnce(Invoke([&moving]() {
                return staircase::MovingPtr{&moving,
                                            [](staircase::IMoving *moving) {}};
            }));
        looper.update(100);
    }

    {
        InSequence s;

        EXPECT_CALL(moving, update(_)).Times(Exactly(1));
        EXPECT_CALL(moving, isTooOld()).WillOnce(Return(true));
        EXPECT_CALL(mUpSensor, hasStateChanged()).WillOnce(Return(false));
        looper.update(100);
    }

    {
        InSequence s;

        EXPECT_CALL(moving, update(_)).Times(Exactly(0));
        EXPECT_CALL(mUpSensor, hasStateChanged()).WillOnce(Return(false));
        looper.update(100);
    }
}

TEST_F(StaircaseLooperTests, OneDownMovingEndBySensor) {
    staircase::StaircaseLooper looper{mBasicLightRefs, mDownSensor, mUpSensor,
                                      mMovingFactory};

    NiceMock<mocks::MovingMock> moving;

    {
        InSequence s;

        EXPECT_CALL(mUpSensor, hasStateChanged()).WillOnce(Return(true));
        EXPECT_CALL(mUpSensor, isClose()).WillOnce(Return(true));
        EXPECT_CALL(mMovingFactory,
                    create(_, staircase::IMoving::Direction::DOWN, _))
            .WillOnce(Invoke([&moving]() {
                return staircase::MovingPtr{&moving,
                                            [](staircase::IMoving *moving) {}};
            }));
        looper.update(100);
    }

    {
        InSequence s;

        EXPECT_CALL(moving, update(100)).Times(Exactly(1));
        EXPECT_CALL(mDownSensor, hasStateChanged()).WillOnce(Return(true));
        EXPECT_CALL(mDownSensor, isClose()).WillOnce(Return(true));
        EXPECT_CALL(moving, isNearEnd()).WillOnce(Return(true));
        EXPECT_CALL(moving, getTimePassed()).Times(Exactly(1));
        EXPECT_CALL(mUpSensor, hasStateChanged()).WillOnce(Return(false));
        looper.update(100);
    }

    {
        InSequence s;

        EXPECT_CALL(moving, update(_)).Times(Exactly(0));
        EXPECT_CALL(mDownSensor, hasStateChanged()).WillOnce(Return(false));
        EXPECT_CALL(mUpSensor, hasStateChanged()).WillOnce(Return(false));
        looper.update(100);
    }
}

} // namespace tests