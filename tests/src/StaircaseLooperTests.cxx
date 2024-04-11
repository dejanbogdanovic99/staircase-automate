#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <mocks/BasicLightMock.hxx>
#include <mocks/MovingDurationCalculatorMock.hxx>
#include <mocks/MovingFactoryMock.hxx>
#include <mocks/MovingMock.hxx>
#include <mocks/MovingTimeFilterMock.hxx>
#include <mocks/PersistenceMock.hxx>
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
using ::testing::Ref;
using ::testing::Return;

class StaircaseLooperInitTests : public ::testing::Test {
  public:
    void SetUp() {}

    void TearDown() {}

    StaircaseLooperInitTests()
        : mBasicLightRefs{mBasicLights[0], mBasicLights[1], mBasicLights[2],
                          mBasicLights[3], mBasicLights[4], mBasicLights[5],
                          mBasicLights[6], mBasicLights[7]},
          mOrigDownFilter{
              std::make_unique<NiceMock<mocks::MovingTimeFilterMock>>()},
          mOrigUpFilter{
              std::make_unique<NiceMock<mocks::MovingTimeFilterMock>>()},
          mDownFilter{*mOrigDownFilter}, mUpFilter{*mOrigUpFilter} {}

  protected:
    std::array<NiceMock<mocks::BasicLightMock>,
               staircase::IBasicLight::kLightsNum>
        mBasicLights;
    staircase::BasicLights mBasicLightRefs;
    NiceMock<mocks::ProximitySensorMock> mDownSensor;
    NiceMock<mocks::ProximitySensorMock> mUpSensor;
    NiceMock<mocks::MovingFactoryMock> mMovingFactory;
    NiceMock<mocks::PersistenceMock> mPersistence;
    NiceMock<mocks::MovingDurationCalculatorMock> mDurationCalculator;
    std::unique_ptr<NiceMock<mocks::MovingTimeFilterMock>> mOrigDownFilter;
    std::unique_ptr<NiceMock<mocks::MovingTimeFilterMock>> mOrigUpFilter;
    NiceMock<mocks::MovingTimeFilterMock> &mDownFilter;
    NiceMock<mocks::MovingTimeFilterMock> &mUpFilter;
};

TEST_F(
    StaircaseLooperInitTests,
    GivenNewStaircaseLooperInstanceIsCreatedNoReadIsPerformedForDownValueIfItDoesntExist) {
    EXPECT_CALL(mPersistence, keyExists(std::string{"dmv"}))
        .WillOnce(Return(false));
    EXPECT_CALL(mPersistence, keyExists(std::string{"umv"}))
        .WillOnce(Return(false));
    EXPECT_CALL(mPersistence, getValue(std::string{"dmv"})).Times(Exactly(0));

    staircase::StaircaseLooper staircaseLooper{mBasicLightRefs,
                                               mDownSensor,
                                               mUpSensor,
                                               mPersistence,
                                               mMovingFactory,
                                               mDurationCalculator,
                                               std::move(mOrigDownFilter),
                                               std::move(mOrigUpFilter)};
}

TEST_F(
    StaircaseLooperInitTests,
    GivenNewStaircaseLooperInstanceIsCreatedNoReadIsPerformedForUpValueIfItDoesntExist) {
    EXPECT_CALL(mPersistence, keyExists(std::string{"dmv"}))
        .WillOnce(Return(false));
    EXPECT_CALL(mPersistence, keyExists(std::string{"umv"}))
        .WillOnce(Return(false));
    EXPECT_CALL(mPersistence, getValue(std::string{"umv"})).Times(Exactly(0));

    staircase::StaircaseLooper staircaseLooper{mBasicLightRefs,
                                               mDownSensor,
                                               mUpSensor,
                                               mPersistence,
                                               mMovingFactory,
                                               mDurationCalculator,
                                               std::move(mOrigDownFilter),
                                               std::move(mOrigUpFilter)};
}

TEST_F(
    StaircaseLooperInitTests,
    GivenNewStaircaseLooperInstanceIsCreatedOldValueIsReadIfDownValueExistsAndFilterIsReset) {
    EXPECT_CALL(mPersistence, keyExists(std::string{"dmv"}))
        .WillOnce(Return(true));
    EXPECT_CALL(mPersistence, keyExists(std::string{"umv"}))
        .WillOnce(Return(false));
    EXPECT_CALL(mPersistence, getValue(std::string{"dmv"}))
        .Times(Exactly(1))
        .WillOnce(Return(10000));
    EXPECT_CALL(mDownFilter, reset(10000)).Times(Exactly(1));

    staircase::StaircaseLooper staircaseLooper{mBasicLightRefs,
                                               mDownSensor,
                                               mUpSensor,
                                               mPersistence,
                                               mMovingFactory,
                                               mDurationCalculator,
                                               std::move(mOrigDownFilter),
                                               std::move(mOrigUpFilter)};
}

TEST_F(
    StaircaseLooperInitTests,
    GivenNewStaircaseLooperInstanceIsCreatedOldValueIsReadIfUpValueExistsAndFilterIsReset) {
    EXPECT_CALL(mPersistence, keyExists(std::string{"dmv"}))
        .WillOnce(Return(false));
    EXPECT_CALL(mPersistence, keyExists(std::string{"umv"}))
        .WillOnce(Return(true));
    EXPECT_CALL(mPersistence, getValue(std::string{"umv"}))
        .Times(Exactly(1))
        .WillOnce(Return(10000));
    EXPECT_CALL(mUpFilter, reset(10000)).Times(Exactly(1));

    staircase::StaircaseLooper staircaseLooper{mBasicLightRefs,
                                               mDownSensor,
                                               mUpSensor,
                                               mPersistence,
                                               mMovingFactory,
                                               mDurationCalculator,
                                               std::move(mOrigDownFilter),
                                               std::move(mOrigUpFilter)};
}

class StaircaseLooperTests : public StaircaseLooperInitTests {
  public:
    void SetUp() {
        ON_CALL(mDownSensor, hasStateChanged()).WillByDefault(Return(false));
        ON_CALL(mUpSensor, hasStateChanged()).WillByDefault(Return(false));
        ON_CALL(mPersistence, keyExists(_)).WillByDefault(Return(false));
    }

    void TearDown() {}

    StaircaseLooperTests()
        : mStaircaseLooper{mBasicLightRefs,
                           mDownSensor,
                           mUpSensor,
                           mPersistence,
                           mMovingFactory,
                           mDurationCalculator,
                           std::move(mOrigDownFilter),
                           std::move(mOrigUpFilter)} {}

  protected:
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
       GivenUpdateIsCalledAndEnoughTimePassedNewValuesAreWrittenToPersistence) {
    constexpr hal::Milliseconds timeInterval = 2 * 60 * 60 * 1000 + 1;
    constexpr hal::Milliseconds timeInterval2 = 2 * 60 * 60 * 1000 - 2;

    EXPECT_CALL(mDownFilter, getCurrentMovingTime()).WillOnce(Return(123));
    EXPECT_CALL(mPersistence, setValue(std::string{"dmv"}, 123))
        .Times(Exactly(1));
    EXPECT_CALL(mUpFilter, getCurrentMovingTime()).WillOnce(Return(321));
    EXPECT_CALL(mPersistence, setValue(std::string{"umv"}, 321))
        .Times(Exactly(1));

    mStaircaseLooper.update(timeInterval);
    mStaircaseLooper.update(timeInterval2);
}

TEST_F(StaircaseLooperTests,
       GivenDownSensorStateChangedToCloseNewUpMovingIsCreated) {
    InSequence s;

    EXPECT_CALL(mDownSensor, hasStateChanged()).WillOnce(Return(true));
    EXPECT_CALL(mDownSensor, isClose()).WillOnce(Return(true));
    EXPECT_CALL(mUpFilter, getCurrentMovingTime()).WillOnce(Return(12000));
    EXPECT_CALL(mMovingFactory,
                create(Ref(mBasicLightRefs), Ref(mDurationCalculator),
                       staircase::IMoving::Direction::UP, 12000))
        .WillOnce(Invoke([]() {
            return staircase::MovingPtr{nullptr,
                                        [](staircase::IMoving *moving) {}};
        }));
    mStaircaseLooper.update(100);
}

TEST_F(StaircaseLooperTests, OneUpMovingToStale) {
    NiceMock<mocks::MovingMock> moving;

    {
        InSequence s;

        EXPECT_CALL(mDownSensor, hasStateChanged()).WillOnce(Return(true));
        EXPECT_CALL(mDownSensor, isClose()).WillOnce(Return(true));
        EXPECT_CALL(mUpFilter, getCurrentMovingTime()).WillOnce(Return(12000));
        EXPECT_CALL(mMovingFactory,
                    create(Ref(mBasicLightRefs), Ref(mDurationCalculator),
                           staircase::IMoving::Direction::UP, 12000))
            .WillOnce(Invoke([&moving]() {
                return staircase::MovingPtr{&moving,
                                            [](staircase::IMoving *moving) {}};
            }));
        mStaircaseLooper.update(100);
    }

    {
        InSequence s;

        EXPECT_CALL(moving, update(100)).Times(Exactly(1));
        EXPECT_CALL(moving, isTooOld()).WillOnce(Return(true));
        EXPECT_CALL(mDownSensor, hasStateChanged()).WillOnce(Return(false));
        mStaircaseLooper.update(100);
    }

    {
        InSequence s;

        EXPECT_CALL(moving, update(_)).Times(Exactly(0));
        EXPECT_CALL(mDownSensor, hasStateChanged()).WillOnce(Return(false));
        mStaircaseLooper.update(100);
    }
}

TEST_F(StaircaseLooperTests, OneUpMovingEndBySensor) {
    NiceMock<mocks::MovingMock> moving;

    {
        InSequence s;

        EXPECT_CALL(mDownSensor, hasStateChanged()).WillOnce(Return(true));
        EXPECT_CALL(mDownSensor, isClose()).WillOnce(Return(true));
        EXPECT_CALL(mUpFilter, getCurrentMovingTime()).WillOnce(Return(12000));
        EXPECT_CALL(mMovingFactory,
                    create(Ref(mBasicLightRefs), Ref(mDurationCalculator),
                           staircase::IMoving::Direction::UP, 12000))
            .WillOnce(Invoke([&moving]() {
                return staircase::MovingPtr{&moving,
                                            [](staircase::IMoving *moving) {}};
            }));
        mStaircaseLooper.update(100);
    }

    {
        InSequence s;

        EXPECT_CALL(moving, update(100)).Times(Exactly(1));
        EXPECT_CALL(mDownSensor, hasStateChanged()).WillOnce(Return(false));
        EXPECT_CALL(mUpSensor, hasStateChanged()).WillOnce(Return(true));
        EXPECT_CALL(mUpSensor, isClose()).WillOnce(Return(true));
        EXPECT_CALL(moving, isNearEnd()).WillOnce(Return(true));
        EXPECT_CALL(moving, getTimePassed()).WillOnce(Return(8000));
        EXPECT_CALL(mUpFilter, processNewMovingTime(8000)).Times(Exactly(1));
        mStaircaseLooper.update(100);
    }

    {
        InSequence s;

        EXPECT_CALL(moving, update(_)).Times(Exactly(0));
        EXPECT_CALL(mDownSensor, hasStateChanged()).WillOnce(Return(false));
        EXPECT_CALL(mUpSensor, hasStateChanged()).WillOnce(Return(false));
        mStaircaseLooper.update(100);
    }
}

TEST_F(StaircaseLooperTests,
       GivenUpSensorStateChangedToCloseNewDownMovingIsCreated) {
    InSequence s;

    EXPECT_CALL(mUpSensor, hasStateChanged()).WillOnce(Return(true));
    EXPECT_CALL(mUpSensor, isClose()).WillOnce(Return(true));
    EXPECT_CALL(mDownFilter, getCurrentMovingTime()).WillOnce(Return(12000));
    EXPECT_CALL(mMovingFactory,
                create(Ref(mBasicLightRefs), Ref(mDurationCalculator),
                       staircase::IMoving::Direction::DOWN, 12000))
        .WillOnce(Invoke([]() {
            return staircase::MovingPtr{nullptr,
                                        [](staircase::IMoving *moving) {}};
        }));
    mStaircaseLooper.update(100);
}

TEST_F(StaircaseLooperTests, OneDownMovingToStale) {
    NiceMock<mocks::MovingMock> moving;

    {
        InSequence s;

        EXPECT_CALL(mUpSensor, hasStateChanged()).WillOnce(Return(true));
        EXPECT_CALL(mUpSensor, isClose()).WillOnce(Return(true));
        EXPECT_CALL(mDownFilter, getCurrentMovingTime())
            .WillOnce(Return(12000));
        EXPECT_CALL(mMovingFactory,
                    create(Ref(mBasicLightRefs), Ref(mDurationCalculator),
                           staircase::IMoving::Direction::DOWN, 12000))
            .WillOnce(Invoke([&moving]() {
                return staircase::MovingPtr{&moving,
                                            [](staircase::IMoving *moving) {}};
            }));
        mStaircaseLooper.update(100);
    }

    {
        InSequence s;

        EXPECT_CALL(moving, update(100)).Times(Exactly(1));
        EXPECT_CALL(moving, isTooOld()).WillOnce(Return(true));
        EXPECT_CALL(mUpSensor, hasStateChanged()).WillOnce(Return(false));
        mStaircaseLooper.update(100);
    }

    {
        InSequence s;

        EXPECT_CALL(moving, update(_)).Times(Exactly(0));
        EXPECT_CALL(mUpSensor, hasStateChanged()).WillOnce(Return(false));
        mStaircaseLooper.update(100);
    }
}

TEST_F(StaircaseLooperTests, OneDownMovingEndBySensor) {
    NiceMock<mocks::MovingMock> moving;

    {
        InSequence s;

        EXPECT_CALL(mUpSensor, hasStateChanged()).WillOnce(Return(true));
        EXPECT_CALL(mUpSensor, isClose()).WillOnce(Return(true));
        EXPECT_CALL(mDownFilter, getCurrentMovingTime())
            .WillOnce(Return(12000));
        EXPECT_CALL(mMovingFactory,
                    create(Ref(mBasicLightRefs), Ref(mDurationCalculator),
                           staircase::IMoving::Direction::DOWN, 12000))
            .WillOnce(Invoke([&moving]() {
                return staircase::MovingPtr{&moving,
                                            [](staircase::IMoving *moving) {}};
            }));
        mStaircaseLooper.update(100);
    }

    {
        InSequence s;

        EXPECT_CALL(moving, update(100)).Times(Exactly(1));
        EXPECT_CALL(mDownSensor, hasStateChanged()).WillOnce(Return(true));
        EXPECT_CALL(mDownSensor, isClose()).WillOnce(Return(true));
        EXPECT_CALL(moving, isNearEnd()).WillOnce(Return(true));
        EXPECT_CALL(moving, getTimePassed()).WillOnce(Return(8000));
        EXPECT_CALL(mDownFilter, processNewMovingTime(8000)).Times(Exactly(1));
        EXPECT_CALL(mUpSensor, hasStateChanged()).WillOnce(Return(false));
        mStaircaseLooper.update(100);
    }

    {
        InSequence s;

        EXPECT_CALL(moving, update(_)).Times(Exactly(0));
        EXPECT_CALL(mDownSensor, hasStateChanged()).WillOnce(Return(false));
        EXPECT_CALL(mUpSensor, hasStateChanged()).WillOnce(Return(false));
        mStaircaseLooper.update(100);
    }
}

} // namespace tests