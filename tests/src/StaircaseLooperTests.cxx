#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <mocks/BasicLightMock.hxx>
#include <mocks/MovingDurationCalculatorMock.hxx>
#include <mocks/MovingFactoryMock.hxx>
#include <mocks/MovingMock.hxx>
#include <mocks/MovingTimeFilterMock.hxx>
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

template <typename T> class ReturnOnce {
  public:
    ReturnOnce(const T &first, const T &other)
        : mFirst{first}, mOther{other}, mIsFirst{true} {}

    template <typename... N> T operator()(N... args) {
        if (mIsFirst) {
            mIsFirst = false;
            return mFirst;
        } else {
            return mOther;
        }
    }

  private:
    T mFirst;
    T mOther;
    bool mIsFirst;
};

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
          mStaircaseLooper{mBasicLightRefs, mDownSensor,         mUpSensor,
                           mMovingFactory,  mDurationCalculator, mDownFilter,
                           mUpFilter} {}

  protected:
    static constexpr hal::Milliseconds kDefaultTime = 123;
    static constexpr hal::Milliseconds kDefaultMovingTime = 12100;

    std::array<NiceMock<mocks::BasicLightMock>,
               staircase::IBasicLight::kLightsNum>
        mBasicLights;
    staircase::BasicLights mBasicLightRefs;
    NiceMock<mocks::ProximitySensorMock> mDownSensor;
    NiceMock<mocks::ProximitySensorMock> mUpSensor;
    NiceMock<mocks::MovingFactoryMock> mMovingFactory;
    NiceMock<mocks::MovingDurationCalculatorMock> mDurationCalculator;
    NiceMock<mocks::MovingTimeFilterMock> mDownFilter;
    NiceMock<mocks::MovingTimeFilterMock> mUpFilter;

    staircase::StaircaseLooper mStaircaseLooper;
};

class StaircaseLooperUpdateTests : public StaircaseLooperTests {};

TEST_F(StaircaseLooperUpdateTests, GIVENUpdateIsCalledTHENItUpdatesAllLights) {
    std::for_each(
        std::begin(mBasicLights), std::end(mBasicLights), [](auto &basicLight) {
            EXPECT_CALL(basicLight, update(kDefaultTime)).Times(Exactly(1));
        });

    mStaircaseLooper.update(kDefaultTime);
}

TEST_F(StaircaseLooperUpdateTests, GIVENUpdateIsCalledTHENItUpdatesUpSensor) {
    EXPECT_CALL(mUpSensor, update(kDefaultTime)).Times(Exactly(1));
    mStaircaseLooper.update(kDefaultTime);
}

TEST_F(StaircaseLooperUpdateTests, GIVENUpdateIsCalledTHENItUpdatesDownSensor) {
    EXPECT_CALL(mDownSensor, update(kDefaultTime)).Times(Exactly(1));
    mStaircaseLooper.update(kDefaultTime);
}

class StaircaseLooperDownMovingCreateTests : public StaircaseLooperTests {};

TEST_F(
    StaircaseLooperDownMovingCreateTests,
    GIVENUpSensorChangesToCloseAndThereAreNoOtherMovingsDuringUpdateTHENNewDownMovingIsCreated) {
    EXPECT_CALL(mUpSensor, hasStateChanged()).WillOnce(Return(true));
    EXPECT_CALL(mUpSensor, isClose()).WillOnce(Return(true));
    EXPECT_CALL(mDownFilter, getCurrentMovingTime())
        .WillOnce(Return(kDefaultMovingTime));
    EXPECT_CALL(mMovingFactory,
                create(Ref(mBasicLightRefs), Ref(mDurationCalculator),
                       staircase::IMoving::Direction::DOWN, kDefaultMovingTime))
        .WillOnce(Return(nullptr));

    mStaircaseLooper.update(kDefaultTime);
}

TEST_F(
    StaircaseLooperDownMovingCreateTests,
    GIVENUpSensorChangesToCloseOnlyOnceAndThereAreNoOtherMovingsDuringUpdateTHENOnlyOneNewDownMovingIsCreated) {
    EXPECT_CALL(mUpSensor, hasStateChanged())
        .WillOnce(Return(true))
        .WillRepeatedly(Return(false));
    EXPECT_CALL(mUpSensor, isClose()).Times(Exactly(1)).WillOnce(Return(true));
    EXPECT_CALL(mDownFilter, getCurrentMovingTime())
        .Times(Exactly(1))
        .WillOnce(Return(kDefaultMovingTime));
    EXPECT_CALL(mMovingFactory,
                create(Ref(mBasicLightRefs), Ref(mDurationCalculator),
                       staircase::IMoving::Direction::DOWN, kDefaultMovingTime))
        .Times(Exactly(1))
        .WillOnce(Return(nullptr));

    mStaircaseLooper.update(kDefaultTime);
    mStaircaseLooper.update(kDefaultTime);
    mStaircaseLooper.update(kDefaultTime);
}

class StaircaseLooperDownMovingCreatedTests : public StaircaseLooperTests {
  public:
    void SetUp() override {
        StaircaseLooperTests::SetUp();
        ON_CALL(mUpSensor, hasStateChanged())
            .WillByDefault(ReturnOnce(true, false));
        ON_CALL(mUpSensor, isClose()).WillByDefault(Return(true));
        ON_CALL(mDownFilter, getCurrentMovingTime())
            .WillByDefault(Return(kDefaultMovingTime));
        ON_CALL(mMovingFactory,
                create(Ref(mBasicLightRefs), Ref(mDurationCalculator),
                       staircase::IMoving::Direction::DOWN, kDefaultMovingTime))
            .WillByDefault(Invoke(
                this, &StaircaseLooperDownMovingCreatedTests::createMovingPtr));

        mStaircaseLooper.update(kDefaultTime);
    }

    staircase::MovingPtr createMovingPtr() noexcept {
        return staircase::MovingPtr{&mMoving, [](staircase::IMoving *) {}};
    }

  protected:
    NiceMock<mocks::MovingMock> mMoving;
};

class StaircaseLooperDownMovingUpdateTests
    : public StaircaseLooperDownMovingCreatedTests {};

TEST_F(StaircaseLooperDownMovingUpdateTests,
       GIVENThereIsOneMovingDuringUpdateTHENThatMovingIsUpdate) {
    EXPECT_CALL(mMoving, update(kDefaultTime)).Times(1);
    mStaircaseLooper.update(kDefaultTime);
}

class StaircaseLooperDownMovingFinishesTests
    : public StaircaseLooperDownMovingCreatedTests {
  public:
    void SetUp() override {
        StaircaseLooperDownMovingCreatedTests::SetUp();
        EXPECT_CALL(mDownSensor, hasStateChanged())
            .WillOnce(Return(true))
            .WillRepeatedly(Return(false));
        EXPECT_CALL(mDownSensor, isClose()).WillOnce(Return(true));
    }
};

TEST_F(
    StaircaseLooperDownMovingFinishesTests,
    GIVENThereIsOneMovingAndDownSensorIsTriggeredAndTheMovingIsNotCloseToTheEndDuringUpdateTHENMovingIsNotFinishedAndIsUpdatedNextTime) {
    EXPECT_CALL(mMoving, isNearEnd()).Times(Exactly(1)).WillOnce(Return(false));
    mStaircaseLooper.update(kDefaultTime);

    EXPECT_CALL(mMoving, update(kDefaultTime)).Times(1);
    mStaircaseLooper.update(kDefaultTime);
}

TEST_F(
    StaircaseLooperDownMovingFinishesTests,
    GIVENThereIsOneMovingAndDownSensorIsTriggeredAndTheMovingIsCloseToTheEndDuringUpdateTHENMovingIsFinished) {
    EXPECT_CALL(mMoving, isNearEnd()).Times(Exactly(1)).WillOnce(Return(true));
    mStaircaseLooper.update(kDefaultTime);

    EXPECT_CALL(mMoving, update(kDefaultTime)).Times(Exactly(0));
    mStaircaseLooper.update(kDefaultTime);
}

TEST_F(StaircaseLooperDownMovingFinishesTests,
       GIVENThereIsOneMovingAndItIsNotStaleTHENItIsUpdated) {
    EXPECT_CALL(mMoving, isTooOld()).Times(2).WillRepeatedly(Return(false));
    mStaircaseLooper.update(kDefaultTime);

    EXPECT_CALL(mMoving, update(kDefaultTime)).Times(1);
    mStaircaseLooper.update(kDefaultTime);
}

TEST_F(StaircaseLooperDownMovingFinishesTests,
       GIVENThereIsOneMovingAndItIsStaleTHENItIsRemoved) {
    EXPECT_CALL(mMoving, isTooOld()).Times(Exactly(1)).WillOnce(Return(true));
    mStaircaseLooper.update(kDefaultTime);

    EXPECT_CALL(mMoving, update(kDefaultTime)).Times(Exactly(0));
    mStaircaseLooper.update(kDefaultTime);
}

class StaircaseLooperDownMovingJustStarted
    : public StaircaseLooperDownMovingCreatedTests {};

TEST_F(
    StaircaseLooperDownMovingJustStarted,
    GIVENNewMovingHasJustBeenCreatedAndItHasJustStartedAndSensorIsCloseTHENNewMovingCannotBeCreated) {
    EXPECT_CALL(mMoving, isNearBegin())
        .Times(Exactly(1))
        .WillOnce(Return(true));
    EXPECT_CALL(mUpSensor, hasStateChanged()).WillOnce(Return(true));
    EXPECT_CALL(mUpSensor, isClose()).WillOnce(Return(true));
    EXPECT_CALL(mMovingFactory, create(_, _, _, _)).Times(Exactly(0));

    mStaircaseLooper.update(kDefaultTime);
}

TEST_F(
    StaircaseLooperDownMovingJustStarted,
    GIVENNewMovingHasJustBeenCreatedAndItHasNotJustStartedAndSensorIsCloseTHENNewMovingIsCreated) {
    NiceMock<mocks::MovingMock> newMoving;
    EXPECT_CALL(mMoving, isNearBegin())
        .Times(Exactly(1))
        .WillOnce(Return(false));
    EXPECT_CALL(mUpSensor, hasStateChanged()).WillOnce(Return(true));
    EXPECT_CALL(mUpSensor, isClose()).WillOnce(Return(true));
    EXPECT_CALL(mDownFilter, getCurrentMovingTime())
        .WillOnce(Return(kDefaultMovingTime));
    EXPECT_CALL(mMovingFactory,
                create(Ref(mBasicLightRefs), Ref(mDurationCalculator),
                       staircase::IMoving::Direction::DOWN, kDefaultMovingTime))
        .WillOnce(Invoke([&]() {
            return staircase::MovingPtr{&newMoving,
                                        [](staircase::IMoving *) {}};
        }));

    mStaircaseLooper.update(kDefaultTime);
}

class StaircaseLooperTwoDownMovingsCreated
    : public StaircaseLooperDownMovingCreatedTests

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