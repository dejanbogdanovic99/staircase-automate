#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <mocks/BinaryValueWriterMock.hxx>

#include <hal/BinaryValue.hxx>
#include <hal/Timing.hxx>

#include <staircase/BasicLight.hxx>

namespace tests {

using ::testing::_;
using ::testing::Exactly;
using ::testing::NiceMock;

class BasicLightTestsInitialization : public ::testing::Test {
  protected:
    NiceMock<mocks::BinaryValueWriterMock> mBinaryValueWriter;
};

TEST_F(BasicLightTestsInitialization, GivenBasicLightIsCreatedWriteLowValue) {
    EXPECT_CALL(mBinaryValueWriter, writeValue(hal::BinaryValue::LOW))
        .Times(Exactly(1));
    staircase::BasicLight basicLight{mBinaryValueWriter};
}

class BasicLightTests : public BasicLightTestsInitialization {
  protected:
    staircase::BasicLight mBasicLight{mBinaryValueWriter};
};

TEST_F(BasicLightTests, GivenBasicLightIsCreatedThereAreNoWritesOnUpdates) {
    EXPECT_CALL(mBinaryValueWriter, writeValue(_)).Times(Exactly(0));

    mBasicLight.update(300);
    mBasicLight.update(1000);
    mBasicLight.update(10000);
    mBasicLight.update(100000);
}

TEST_F(BasicLightTests, GiveTurnOnIsCalledBasicLightWritesANewValue) {
    EXPECT_CALL(mBinaryValueWriter, writeValue(hal::BinaryValue::HIGH))
        .Times(Exactly(1));

    mBasicLight.turnOn();
}

TEST_F(
    BasicLightTests,
    GivenTurnOnIsCalledWithDefaultValueNewWriteIsNotCalledIfTimePassedIsLessThenDefaultValue) {
    mBasicLight.turnOn();

    EXPECT_CALL(mBinaryValueWriter, writeValue(hal::BinaryValue::LOW))
        .Times(Exactly(0));

    mBasicLight.update(550);
}

TEST_F(
    BasicLightTests,
    GivenTurnOnIsCalledWithDefaultValueNewWriteIsNotCalledIfTimePassedIsLessThenDefaultValueMultipleTimes) {
    mBasicLight.turnOn();

    EXPECT_CALL(mBinaryValueWriter, writeValue(hal::BinaryValue::LOW))
        .Times(Exactly(0));

    mBasicLight.update(550);
    mBasicLight.update(550);
}

TEST_F(
    BasicLightTests,
    GivenTurnOnIsCalledWithDefaultValueNewWriteIsCalledIfTimePassedIsGreaterThenDefaultValue) {
    mBasicLight.turnOn();

    EXPECT_CALL(mBinaryValueWriter, writeValue(hal::BinaryValue::LOW))
        .Times(Exactly(1));

    mBasicLight.update(550);
    mBasicLight.update(1500);
    mBasicLight.update(1500);
}

TEST_F(
    BasicLightTests,
    GivenTurnOnIsCalledNewWriteIsNotCalledIfTimePassedIsLessThenExpectedValue) {
    mBasicLight.turnOn(1000);

    EXPECT_CALL(mBinaryValueWriter, writeValue(hal::BinaryValue::LOW))
        .Times(Exactly(0));

    mBasicLight.update(550);
}

TEST_F(
    BasicLightTests,
    GivenTurnOnIsCalledNewWriteIsNotCalledIfTimePassedIsLessThenExpectedValueMultipleTimes) {
    mBasicLight.turnOn(1000);

    EXPECT_CALL(mBinaryValueWriter, writeValue(hal::BinaryValue::LOW))
        .Times(Exactly(0));

    mBasicLight.update(550);
    mBasicLight.update(320);
}

TEST_F(
    BasicLightTests,
    GivenTurnOnIsCalledNewWriteIsCalledIfTimePassedIsGreaterThenExpectedValueMultipleTimes) {
    mBasicLight.turnOn(1000);

    EXPECT_CALL(mBinaryValueWriter, writeValue(hal::BinaryValue::LOW))
        .Times(Exactly(1));

    mBasicLight.update(550);
    mBasicLight.update(550);
}

TEST_F(BasicLightTests, GivenTurnOnIsCalledForeverNewWriteIsNeverCalled) {
    mBasicLight.turnOn(hal::kForever);

    EXPECT_CALL(mBinaryValueWriter, writeValue(hal::BinaryValue::LOW))
        .Times(Exactly(0));

    mBasicLight.update(550);
    mBasicLight.update(1500);
    mBasicLight.update(1500);
    mBasicLight.update(15000);
    mBasicLight.update(100000);
}

TEST_F(BasicLightTests,
       GivenTurnOnIsCalledWithDefaultValueTurnOffWritesANewValue) {
    mBasicLight.turnOn();

    EXPECT_CALL(mBinaryValueWriter, writeValue(hal::BinaryValue::LOW))
        .Times(Exactly(1));

    mBasicLight.turnOff();
}

TEST_F(BasicLightTests, GivenTurnOnIsCalledForeverTurnOffWritesANewValue) {
    mBasicLight.turnOn(hal::kForever);

    EXPECT_CALL(mBinaryValueWriter, writeValue(hal::BinaryValue::LOW))
        .Times(Exactly(1));

    mBasicLight.turnOff();
}

TEST_F(BasicLightTests, GivenBasicLightIsCreatedItsValueIsOff) {
    EXPECT_TRUE(mBasicLight.isOff());
    EXPECT_FALSE(mBasicLight.isOn());
}

TEST_F(BasicLightTests, GivenBasicLightIsTurnedOnItsValueIsOn) {
    mBasicLight.turnOn();
    EXPECT_TRUE(mBasicLight.isOn());
    EXPECT_FALSE(mBasicLight.isOff());
}

TEST_F(BasicLightTests, GivenBasicLightIsTurnedOnForeverItsValueIsOn) {
    mBasicLight.turnOn(hal::kForever);
    EXPECT_TRUE(mBasicLight.isOn());
    EXPECT_FALSE(mBasicLight.isOff());
}

TEST_F(BasicLightTests, GivenBasicLightIsTurnedOnButDecaysItsValueIsOff) {
    mBasicLight.turnOn(1000);

    mBasicLight.update(1500);
    EXPECT_TRUE(mBasicLight.isOff());
    EXPECT_FALSE(mBasicLight.isOn());
}

TEST_F(BasicLightTests, GivenBasicLightIsTurnedOnAndOffItsValueIsOff) {
    mBasicLight.turnOn();

    mBasicLight.turnOff();
    EXPECT_TRUE(mBasicLight.isOff());
    EXPECT_FALSE(mBasicLight.isOn());
}

} // namespace tests