#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <mocks/BinaryValueWriterMock.hxx>

#include <hal/BinaryValue.hxx>
#include <hal/Timing.hxx>
#include <staircase/BasicLight.hxx>

namespace tests {

using ::testing::_;
using ::testing::Exactly;
using ::testing::InSequence;
using ::testing::NiceMock;

class BasicLightTests : public ::testing::Test {
  public:
    void SetUp() {}

    void TearDown() {}

  protected:
    NiceMock<mocks::BinaryValueWriterMock> mBinaryValueWriter;
};

TEST_F(BasicLightTests, PlainInitializationOff) {
    EXPECT_CALL(mBinaryValueWriter, writeValue(hal::BinaryValue::LOW))
        .Times(Exactly(1));
    staircase::BasicLight basicLight{mBinaryValueWriter};
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::OFF);
}

TEST_F(BasicLightTests, PlainInitializationOffWithTime) {
    EXPECT_CALL(mBinaryValueWriter, writeValue(hal::BinaryValue::LOW))
        .Times(Exactly(1));
    staircase::BasicLight basicLight{
        mBinaryValueWriter, staircase::BasicLight::LightState::OFF, 3000};
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::OFF);
}

TEST_F(BasicLightTests, PlainInitializationOn) {
    EXPECT_CALL(mBinaryValueWriter, writeValue(hal::BinaryValue::HIGH))
        .Times(Exactly(1));
    staircase::BasicLight basicLight{mBinaryValueWriter,
                                     staircase::BasicLight::LightState::ON};
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::ON);
}

TEST_F(BasicLightTests, PlainInitializationOnWithTime) {
    EXPECT_CALL(mBinaryValueWriter, writeValue(hal::BinaryValue::HIGH))
        .Times(Exactly(1));
    staircase::BasicLight basicLight{
        mBinaryValueWriter, staircase::BasicLight::LightState::ON, 3000};
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::ON);
}

TEST_F(BasicLightTests, PlainInitializationOffWithOneUpdate) {
    staircase::BasicLight basicLight{mBinaryValueWriter};

    EXPECT_CALL(mBinaryValueWriter, writeValue(_)).Times(Exactly(0));
    basicLight.update(300);
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::OFF);
}

TEST_F(BasicLightTests, InitializationOnWithOneUpdate) {
    staircase::BasicLight basicLight{mBinaryValueWriter,
                                     staircase::BasicLight::LightState::ON};

    EXPECT_CALL(mBinaryValueWriter, writeValue(_)).Times(Exactly(0));
    basicLight.update(300);
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::ON);
}

TEST_F(BasicLightTests, InitializationOffWithOneUpdateAndChangedState) {
    staircase::BasicLight basicLight{
        mBinaryValueWriter, staircase::BasicLight::LightState::OFF, 200};

    EXPECT_CALL(mBinaryValueWriter, writeValue(hal::BinaryValue::HIGH))
        .Times(Exactly(1));
    basicLight.update(300);
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::ON);
}

TEST_F(BasicLightTests, InitializationOnWithOneUpdateAndChangedState) {
    staircase::BasicLight basicLight{
        mBinaryValueWriter, staircase::BasicLight::LightState::ON, 250};

    EXPECT_CALL(mBinaryValueWriter, writeValue(hal::BinaryValue::LOW))
        .Times(Exactly(1));
    basicLight.update(300);
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::OFF);
}

TEST_F(BasicLightTests, OnUpdateTurnOnNoChange) {
    staircase::BasicLight basicLight{
        mBinaryValueWriter, staircase::BasicLight::LightState::ON, 500};

    EXPECT_CALL(mBinaryValueWriter, writeValue(_)).Times(Exactly(0));
    basicLight.update(300);
    basicLight.turnOn();
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::ON);
}

TEST_F(BasicLightTests, OnUpdateOffTurnOn) {
    staircase::BasicLight basicLight{
        mBinaryValueWriter, staircase::BasicLight::LightState::ON, 250};

    InSequence s;

    EXPECT_CALL(mBinaryValueWriter, writeValue(hal::BinaryValue::LOW))
        .Times(Exactly(1));
    basicLight.update(300);
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::OFF);
    EXPECT_CALL(mBinaryValueWriter, writeValue(hal::BinaryValue::HIGH))
        .Times(Exactly(1));
    basicLight.turnOn();
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::ON);
}

TEST_F(BasicLightTests, OffTurnOnMultipleUpdatesOff) {
    staircase::BasicLight basicLight{mBinaryValueWriter};

    InSequence s;

    EXPECT_CALL(mBinaryValueWriter, writeValue(hal::BinaryValue::HIGH))
        .Times(Exactly(1));
    basicLight.turnOn(1000);
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::ON);
    basicLight.update(550);
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::ON);
    EXPECT_CALL(mBinaryValueWriter, writeValue(hal::BinaryValue::LOW))
        .Times(Exactly(1));
    basicLight.update(550);
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::OFF);
}

TEST_F(BasicLightTests, OffTurnOnMultipleUpdatesOnAgainMultipleUpdatesOff) {
    staircase::BasicLight basicLight{mBinaryValueWriter};

    InSequence s;

    EXPECT_CALL(mBinaryValueWriter, writeValue(hal::BinaryValue::HIGH))
        .Times(Exactly(1));
    basicLight.turnOn(1000);
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::ON);
    basicLight.update(550);
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::ON);
    basicLight.update(300);
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::ON);
    basicLight.turnOn(400);
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::ON);
    EXPECT_CALL(mBinaryValueWriter, writeValue(hal::BinaryValue::LOW))
        .Times(Exactly(1));
    basicLight.update(400);
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::OFF);
}

TEST_F(BasicLightTests, OffTurnOnUpdateTurnOnWithLess) {
    staircase::BasicLight basicLight{mBinaryValueWriter};

    InSequence s;

    EXPECT_CALL(mBinaryValueWriter, writeValue(hal::BinaryValue::HIGH))
        .Times(Exactly(1));
    basicLight.turnOn(1000);
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::ON);
    basicLight.update(550);
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::ON);
    basicLight.turnOn(300);
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::ON);
    basicLight.update(300);
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::ON);
    EXPECT_CALL(mBinaryValueWriter, writeValue(hal::BinaryValue::LOW))
        .Times(Exactly(1));
    basicLight.update(300);
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::OFF);
}

TEST_F(BasicLightTests, OffUpdateTurnOffNoChange) {
    staircase::BasicLight basicLight{
        mBinaryValueWriter, staircase::BasicLight::LightState::OFF, 500};

    EXPECT_CALL(mBinaryValueWriter, writeValue(_)).Times(Exactly(0));
    basicLight.update(300);
    basicLight.turnOff();
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::OFF);
}

TEST_F(BasicLightTests, OffUpdateOnTurnOff) {
    staircase::BasicLight basicLight{
        mBinaryValueWriter, staircase::BasicLight::LightState::OFF, 250};

    InSequence s;

    EXPECT_CALL(mBinaryValueWriter, writeValue(hal::BinaryValue::HIGH))
        .Times(Exactly(1));
    basicLight.update(300);
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::ON);
    EXPECT_CALL(mBinaryValueWriter, writeValue(hal::BinaryValue::LOW))
        .Times(Exactly(1));
    basicLight.turnOff();
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::OFF);
}

TEST_F(BasicLightTests, OnTurnOffMultipleUpdatesOn) {
    staircase::BasicLight basicLight{mBinaryValueWriter,
                                     staircase::BasicLight::LightState::ON};

    InSequence s;

    EXPECT_CALL(mBinaryValueWriter, writeValue(hal::BinaryValue::LOW))
        .Times(Exactly(1));
    basicLight.turnOff(1000);
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::OFF);
    basicLight.update(550);
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::OFF);
    EXPECT_CALL(mBinaryValueWriter, writeValue(hal::BinaryValue::HIGH))
        .Times(Exactly(1));
    basicLight.update(550);
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::ON);
}

TEST_F(BasicLightTests, OnTurnOffMultipleUpdatesOffAgainMultipleUpdatesOn) {
    staircase::BasicLight basicLight{mBinaryValueWriter,
                                     staircase::BasicLight::LightState::ON};

    InSequence s;

    EXPECT_CALL(mBinaryValueWriter, writeValue(hal::BinaryValue::LOW))
        .Times(Exactly(1));
    basicLight.turnOff(1000);
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::OFF);
    basicLight.update(550);
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::OFF);
    basicLight.update(300);
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::OFF);
    basicLight.turnOff(400);
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::OFF);
    EXPECT_CALL(mBinaryValueWriter, writeValue(hal::BinaryValue::HIGH))
        .Times(Exactly(1));
    basicLight.update(400);
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::ON);
}

TEST_F(BasicLightTests, OnTurnOffUpdateTurnOffWithLess) {
    staircase::BasicLight basicLight{mBinaryValueWriter,
                                     staircase::BasicLight::LightState::ON};

    InSequence s;

    EXPECT_CALL(mBinaryValueWriter, writeValue(hal::BinaryValue::LOW))
        .Times(Exactly(1));
    basicLight.turnOff(1000);
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::OFF);
    basicLight.update(550);
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::OFF);
    basicLight.turnOff(300);
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::OFF);
    basicLight.update(300);
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::OFF);
    EXPECT_CALL(mBinaryValueWriter, writeValue(hal::BinaryValue::HIGH))
        .Times(Exactly(1));
    basicLight.update(300);
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::ON);
}

TEST_F(BasicLightTests, OnUpdateTurnOffForeverMultipleUpdates) {
    staircase::BasicLight basicLight{mBinaryValueWriter,
                                     staircase::BasicLight::LightState::ON};

    InSequence s;

    basicLight.update(550);
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::ON);
    EXPECT_CALL(mBinaryValueWriter, writeValue(hal::BinaryValue::LOW))
        .Times(Exactly(1));
    basicLight.turnOff();
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::OFF);
    basicLight.update(300);
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::OFF);
    basicLight.update(3000);
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::OFF);
}

TEST_F(BasicLightTests, TurnOnUpdateOffContinueUpdate) {
    staircase::BasicLight basicLight{mBinaryValueWriter};

    InSequence s;

    EXPECT_CALL(mBinaryValueWriter, writeValue(hal::BinaryValue::HIGH))
        .Times(Exactly(1));
    basicLight.turnOn(1000);
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::ON);
    basicLight.update(550);
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::ON);
    EXPECT_CALL(mBinaryValueWriter, writeValue(hal::BinaryValue::LOW))
        .Times(Exactly(1));
    basicLight.update(500);
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::OFF);
    basicLight.update(1000);
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::OFF);
    basicLight.update(1000);
    EXPECT_EQ(basicLight.getState(), staircase::BasicLight::LightState::OFF);
}

// TODO: add specific use case tests

} // namespace tests