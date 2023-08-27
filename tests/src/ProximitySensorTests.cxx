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

class ProximitySensorTests : public ::testing::Test {
  public:
    void SetUp() {}

    void TearDown() {}

  protected:
    NiceMock<mocks::BinaryValueReaderMock> mBinaryValueReader;
};

TEST_F(ProximitySensorTests, InitializationWithFar) {
    EXPECT_CALL(mBinaryValueReader, readValue())
        .WillOnce(Return(hal::BinaryValue::LOW));
    staircase::ProximitySensor proximitySensor{mBinaryValueReader};

    EXPECT_TRUE(proximitySensor.isFar());
    EXPECT_FALSE(proximitySensor.isClose());
    EXPECT_FALSE(proximitySensor.stateChanged());
}

TEST_F(ProximitySensorTests, InitializationWithClose) {
    EXPECT_CALL(mBinaryValueReader, readValue())
        .WillOnce(Return(hal::BinaryValue::HIGH));
    staircase::ProximitySensor proximitySensor{mBinaryValueReader};

    EXPECT_TRUE(proximitySensor.isClose());
    EXPECT_FALSE(proximitySensor.isFar());
    EXPECT_FALSE(proximitySensor.stateChanged());
}

TEST_F(ProximitySensorTests, FarToCloseUpdateStateTooFast) {
    EXPECT_CALL(mBinaryValueReader, readValue())
        .WillOnce(Return(hal::BinaryValue::LOW));
    staircase::ProximitySensor proximitySensor{mBinaryValueReader};

    InSequence s;

    EXPECT_CALL(mBinaryValueReader, readValue())
        .WillOnce(Return(hal::BinaryValue::HIGH));
    proximitySensor.update(100);
    EXPECT_TRUE(proximitySensor.isFar());
    EXPECT_FALSE(proximitySensor.isClose());
    EXPECT_FALSE(proximitySensor.stateChanged());

    EXPECT_CALL(mBinaryValueReader, readValue())
        .WillOnce(Return(hal::BinaryValue::HIGH));
    proximitySensor.update(100);
    EXPECT_TRUE(proximitySensor.isFar());
    EXPECT_FALSE(proximitySensor.isClose());
    EXPECT_FALSE(proximitySensor.stateChanged());

    EXPECT_CALL(mBinaryValueReader, readValue())
        .WillOnce(Return(hal::BinaryValue::LOW));
    proximitySensor.update(100);
    EXPECT_TRUE(proximitySensor.isFar());
    EXPECT_FALSE(proximitySensor.isClose());
    EXPECT_FALSE(proximitySensor.stateChanged());
}

TEST_F(ProximitySensorTests, FarToClosSuccessfullUpdate) {
    EXPECT_CALL(mBinaryValueReader, readValue())
        .WillOnce(Return(hal::BinaryValue::LOW));
    staircase::ProximitySensor proximitySensor{mBinaryValueReader};

    InSequence s;

    EXPECT_CALL(mBinaryValueReader, readValue())
        .WillOnce(Return(hal::BinaryValue::HIGH));
    proximitySensor.update(100);
    EXPECT_TRUE(proximitySensor.isFar());
    EXPECT_FALSE(proximitySensor.isClose());
    EXPECT_FALSE(proximitySensor.stateChanged());

    EXPECT_CALL(mBinaryValueReader, readValue())
        .WillOnce(Return(hal::BinaryValue::HIGH));
    proximitySensor.update(280);
    EXPECT_TRUE(proximitySensor.isClose());
    EXPECT_FALSE(proximitySensor.isFar());
    EXPECT_TRUE(proximitySensor.stateChanged());

    EXPECT_CALL(mBinaryValueReader, readValue())
        .WillOnce(Return(hal::BinaryValue::HIGH));
    proximitySensor.update(100);
    EXPECT_TRUE(proximitySensor.isClose());
    EXPECT_FALSE(proximitySensor.isFar());
    EXPECT_FALSE(proximitySensor.stateChanged());
}

TEST_F(ProximitySensorTests, FarCloseInvalidUpdatesToClosSuccessfullUpdate) {
    EXPECT_CALL(mBinaryValueReader, readValue())
        .WillOnce(Return(hal::BinaryValue::LOW));
    staircase::ProximitySensor proximitySensor{mBinaryValueReader};

    InSequence s;

    EXPECT_CALL(mBinaryValueReader, readValue())
        .WillOnce(Return(hal::BinaryValue::HIGH));
    proximitySensor.update(100);
    EXPECT_TRUE(proximitySensor.isFar());
    EXPECT_FALSE(proximitySensor.isClose());
    EXPECT_FALSE(proximitySensor.stateChanged());

    EXPECT_CALL(mBinaryValueReader, readValue())
        .WillOnce(Return(hal::BinaryValue::HIGH));
    proximitySensor.update(100);
    EXPECT_TRUE(proximitySensor.isFar());
    EXPECT_FALSE(proximitySensor.isClose());
    EXPECT_FALSE(proximitySensor.stateChanged());

    EXPECT_CALL(mBinaryValueReader, readValue())
        .WillOnce(Return(hal::BinaryValue::LOW));
    proximitySensor.update(100);
    EXPECT_TRUE(proximitySensor.isFar());
    EXPECT_FALSE(proximitySensor.isClose());
    EXPECT_FALSE(proximitySensor.stateChanged());

    EXPECT_CALL(mBinaryValueReader, readValue())
        .WillOnce(Return(hal::BinaryValue::HIGH));
    proximitySensor.update(100);
    EXPECT_TRUE(proximitySensor.isFar());
    EXPECT_FALSE(proximitySensor.isClose());
    EXPECT_FALSE(proximitySensor.stateChanged());

    EXPECT_CALL(mBinaryValueReader, readValue())
        .WillOnce(Return(hal::BinaryValue::HIGH));
    proximitySensor.update(280);
    EXPECT_TRUE(proximitySensor.isClose());
    EXPECT_FALSE(proximitySensor.isFar());
    EXPECT_TRUE(proximitySensor.stateChanged());

    EXPECT_CALL(mBinaryValueReader, readValue())
        .WillOnce(Return(hal::BinaryValue::HIGH));
    proximitySensor.update(100);
    EXPECT_TRUE(proximitySensor.isClose());
    EXPECT_FALSE(proximitySensor.isFar());
    EXPECT_FALSE(proximitySensor.stateChanged());
}

TEST_F(ProximitySensorTests, FarClosFarSuccessfullUpdate) {
    EXPECT_CALL(mBinaryValueReader, readValue())
        .WillOnce(Return(hal::BinaryValue::LOW));
    staircase::ProximitySensor proximitySensor{mBinaryValueReader};

    InSequence s;

    EXPECT_CALL(mBinaryValueReader, readValue())
        .WillOnce(Return(hal::BinaryValue::HIGH));
    proximitySensor.update(100);
    EXPECT_TRUE(proximitySensor.isFar());
    EXPECT_FALSE(proximitySensor.isClose());
    EXPECT_FALSE(proximitySensor.stateChanged());

    EXPECT_CALL(mBinaryValueReader, readValue())
        .WillOnce(Return(hal::BinaryValue::HIGH));
    proximitySensor.update(280);
    EXPECT_TRUE(proximitySensor.isClose());
    EXPECT_FALSE(proximitySensor.isFar());
    EXPECT_TRUE(proximitySensor.stateChanged());

    EXPECT_CALL(mBinaryValueReader, readValue())
        .WillOnce(Return(hal::BinaryValue::HIGH));
    proximitySensor.update(100);
    EXPECT_TRUE(proximitySensor.isClose());
    EXPECT_FALSE(proximitySensor.isFar());
    EXPECT_FALSE(proximitySensor.stateChanged());

    EXPECT_CALL(mBinaryValueReader, readValue())
        .WillOnce(Return(hal::BinaryValue::LOW));
    proximitySensor.update(100);
    EXPECT_TRUE(proximitySensor.isClose());
    EXPECT_FALSE(proximitySensor.isFar());
    EXPECT_FALSE(proximitySensor.stateChanged());

    EXPECT_CALL(mBinaryValueReader, readValue())
        .WillOnce(Return(hal::BinaryValue::LOW));
    proximitySensor.update(280);
    EXPECT_TRUE(proximitySensor.isFar());
    EXPECT_FALSE(proximitySensor.isClose());
    EXPECT_TRUE(proximitySensor.stateChanged());

    EXPECT_CALL(mBinaryValueReader, readValue())
        .WillOnce(Return(hal::BinaryValue::LOW));
    proximitySensor.update(100);
    EXPECT_TRUE(proximitySensor.isFar());
    EXPECT_FALSE(proximitySensor.isClose());
    EXPECT_FALSE(proximitySensor.stateChanged());
}

} // namespace tests