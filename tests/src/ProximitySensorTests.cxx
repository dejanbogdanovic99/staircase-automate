#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <mocks/BinaryValueReaderMock.hxx>

#include <hal/BinaryValue.hxx>
#include <hal/Timing.hxx>

#include <staircase/ProximitySensor.hxx>

namespace tests {

using ::testing::_;
using ::testing::Exactly;
using ::testing::InSequence;
using ::testing::NiceMock;
using ::testing::Return;

class ProximitySensorTestsInitialization : public ::testing::Test {
  protected:
    NiceMock<mocks::BinaryValueReaderMock> mBinaryValueReader;
};

TEST_F(ProximitySensorTestsInitialization,
       GivenProximitySensorIsInitializedWithLowValueItIsFarAndHasNotChanged) {
    EXPECT_CALL(mBinaryValueReader, readValue())
        .WillOnce(Return(hal::BinaryValue::LOW));
    staircase::ProximitySensor proximitySensor{mBinaryValueReader};

    EXPECT_TRUE(proximitySensor.isFar());
    EXPECT_FALSE(proximitySensor.isClose());
    EXPECT_FALSE(proximitySensor.hasStateChanged());
}

TEST_F(
    ProximitySensorTestsInitialization,
    GivenProximitySensorIsInitializedWithHighValueItIsCloseAndHasNotChanged) {
    EXPECT_CALL(mBinaryValueReader, readValue())
        .WillOnce(Return(hal::BinaryValue::HIGH));
    staircase::ProximitySensor proximitySensor{mBinaryValueReader};

    EXPECT_TRUE(proximitySensor.isClose());
    EXPECT_FALSE(proximitySensor.isFar());
    EXPECT_FALSE(proximitySensor.hasStateChanged());
}

class ProximitySensorTestsInitialStateFar
    : public ProximitySensorTestsInitialization {

    void SetUp() override {
        ON_CALL(mBinaryValueReader, readValue())
            .WillByDefault(Return(hal::BinaryValue::LOW));
    }
};

TEST_F(
    ProximitySensorTestsInitialStateFar,
    GivenStateChangesToCloseButNotEnoughTimePassesStateIsStillFarAndItHasNotChanged) {

    staircase::ProximitySensor proximitySensor{mBinaryValueReader};

    EXPECT_CALL(mBinaryValueReader, readValue())
        .WillOnce(Return(hal::BinaryValue::HIGH));
    proximitySensor.update(100);
    EXPECT_TRUE(proximitySensor.isFar());
    EXPECT_FALSE(proximitySensor.isClose());
    EXPECT_FALSE(proximitySensor.hasStateChanged());
}

TEST_F(
    ProximitySensorTestsInitialStateFar,
    GivenStateChangesToCloseAndEnoughTimePassesStateBecomesCloseAndItHasChanged) {

    staircase::ProximitySensor proximitySensor{mBinaryValueReader};

    EXPECT_CALL(mBinaryValueReader, readValue()).Times(3)
        .WillRepeatedly(Return(hal::BinaryValue::HIGH));
    proximitySensor.update(130);
    proximitySensor.update(130);
    proximitySensor.update(130);
    EXPECT_FALSE(proximitySensor.isFar());
    EXPECT_TRUE(proximitySensor.isClose());
    EXPECT_TRUE(proximitySensor.hasStateChanged());
}

class ProximitySensorTestsInitialStateClose
    : public ProximitySensorTestsInitialization {

    void SetUp() override {
        ON_CALL(mBinaryValueReader, readValue())
            .WillByDefault(Return(hal::BinaryValue::HIGH));
    }
};

TEST_F(
    ProximitySensorTestsInitialStateClose,
    GivenStateChangesToFarButNotEnoughTimePassesStateIsStillCloseAndItHasNotChanged) {

    staircase::ProximitySensor proximitySensor{mBinaryValueReader};

    EXPECT_CALL(mBinaryValueReader, readValue())
        .WillOnce(Return(hal::BinaryValue::LOW));
    proximitySensor.update(100);
    EXPECT_TRUE(proximitySensor.isClose());
    EXPECT_FALSE(proximitySensor.isFar());
    EXPECT_FALSE(proximitySensor.hasStateChanged());
}

TEST_F(
    ProximitySensorTestsInitialStateClose,
    GivenStateChangesToFarAndEnoughTimePassesStateBecomesFarAndItHasChanged) {

    staircase::ProximitySensor proximitySensor{mBinaryValueReader};

    EXPECT_CALL(mBinaryValueReader, readValue()).Times(3)
        .WillRepeatedly(Return(hal::BinaryValue::LOW));
    proximitySensor.update(130);
    proximitySensor.update(130);
    proximitySensor.update(130);
    EXPECT_FALSE(proximitySensor.isClose());
    EXPECT_TRUE(proximitySensor.isFar());
    EXPECT_TRUE(proximitySensor.hasStateChanged());
}

} // namespace tests