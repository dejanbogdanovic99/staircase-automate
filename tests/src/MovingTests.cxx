#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <mocks/BasicLightMock.hxx>

#include <hal/Timing.hxx>
#include <staircase/Moving.hxx>

#include <algorithm>
#include <array>

namespace tests {

using ::testing::_;
using ::testing::Exactly;
using ::testing::InSequence;
using ::testing::NiceMock;

class MovingTests : public ::testing::Test {
  public:
    void SetUp() {
        std::transform(std::begin(mBasicLights), std::end(mBasicLights),
                       std::begin(mBasicLightPointers),
                       [](auto &basicLight) { return &basicLight; });
        std::transform(std::rbegin(mBasicLights), std::rend(mBasicLights),
                       std::begin(mBasicLightReversePointers),
                       [](auto &basicLight) { return &basicLight; });
    }

    void TearDown() {}

  protected:
    std::array<NiceMock<mocks::BasicLightMock>,
               staircase::IBasicLight::kLightNum>
        mBasicLights;
    std::array<staircase::IBasicLight *, staircase::IBasicLight::kLightNum>
        mBasicLightPointers;
    std::array<staircase::IBasicLight *, staircase::IBasicLight::kLightNum>
        mBasicLightReversePointers;
};

TEST_F(MovingTests, Initialization) {
    EXPECT_CALL(mBasicLights[0], turnOn(500)).Times(Exactly(1));
    staircase::Moving moving{
        staircase::IBasicLight::BasicLights{std::begin(mBasicLightPointers),
                                            std::end(mBasicLightPointers)},
        staircase::Moving::Direction::UP, 12000};
}

TEST_F(MovingTests, InitializationReverse) {
    EXPECT_CALL(mBasicLights[staircase::IBasicLight::kLightNum - 1],
                turnOn(500))
        .Times(Exactly(1));
    staircase::Moving moving{staircase::IBasicLight::BasicLights{
                                 std::begin(mBasicLightReversePointers),
                                 std::end(mBasicLightReversePointers)},
                             staircase::Moving::Direction::DOWN, 12000};
}

TEST_F(MovingTests, TimePassedCalculation) {
    staircase::Moving moving{
        staircase::IBasicLight::BasicLights{std::begin(mBasicLightPointers),
                                            std::end(mBasicLightPointers)},
        staircase::Moving::Direction::UP, 12000};

    InSequence s;

    moving.update(300); // 300
    EXPECT_EQ(moving.getTimePassed(), 300);
    moving.update(400); // 700
    EXPECT_EQ(moving.getTimePassed(), 700);
    moving.update(500); // 1200
    EXPECT_EQ(moving.getTimePassed(), 1200);
    moving.update(500); // 1700
    EXPECT_EQ(moving.getTimePassed(), 1700);
}

TEST_F(MovingTests, NearBeginCalculation) {
    staircase::Moving moving{staircase::IBasicLight::BasicLights{
                                 std::begin(mBasicLightReversePointers),
                                 std::end(mBasicLightReversePointers)},
                             staircase::Moving::Direction::UP, 12000};

    InSequence s;

    moving.update(300); // 300
    EXPECT_TRUE(moving.isNearBegin());
    moving.update(400); // 700
    EXPECT_TRUE(moving.isNearBegin());
    moving.update(500); // 1200
    EXPECT_TRUE(moving.isNearBegin());
    moving.update(500); // 1700
    EXPECT_TRUE(moving.isNearBegin());
    moving.update(500); // 2200
    EXPECT_FALSE(moving.isNearBegin());
    moving.update(500); // 2700
    EXPECT_FALSE(moving.isNearBegin());
}

TEST_F(MovingTests, NearEndCalculation) {
    staircase::Moving moving{
        staircase::IBasicLight::BasicLights{std::begin(mBasicLightPointers),
                                            std::end(mBasicLightPointers)},
        staircase::Moving::Direction::DOWN, 12100};

    InSequence s;

    EXPECT_FALSE(moving.isNearEnd());
    moving.update(300); // 300
    EXPECT_FALSE(moving.isNearEnd());
    moving.update(5000); // 5300
    EXPECT_FALSE(moving.isNearEnd());
    moving.update(4000); // 9300
    EXPECT_FALSE(moving.isNearEnd());
    moving.update(801); // 10101
    EXPECT_TRUE(moving.isNearEnd());
    moving.update(1200); // 11301
    EXPECT_TRUE(moving.isNearEnd());
    moving.update(1200); // 12501
    EXPECT_TRUE(moving.isNearEnd());
    moving.update(1300); // 13801
    EXPECT_TRUE(moving.isNearEnd());
    moving.update(300); // 14101
    EXPECT_FALSE(moving.isNearEnd());
    moving.update(300); // 14401
    EXPECT_FALSE(moving.isNearEnd());
}

TEST_F(MovingTests, LightTurningOnLightOne) {
    staircase::Moving moving{
        staircase::IBasicLight::BasicLights{std::begin(mBasicLightPointers),
                                            std::end(mBasicLightPointers)},
        staircase::Moving::Direction::DOWN, 12000};

    InSequence s;

    moving.update(300); // 300
    EXPECT_CALL(mBasicLights[1], turnOn(500)).Times(Exactly(1));
    moving.update(300); // 600
    EXPECT_CALL(mBasicLights[1], turnOn(_)).Times(Exactly(0));
    moving.update(300);  // 900
    moving.update(300);  // 1200
    moving.update(4000); // 5200
}

TEST_F(MovingTests, LightTurningOnLightTwo) {
    staircase::Moving moving{staircase::IBasicLight::BasicLights{
                                 std::begin(mBasicLightReversePointers),
                                 std::end(mBasicLightReversePointers)},
                             staircase::Moving::Direction::DOWN, 12000};

    InSequence s;

    moving.update(300); // 300
    moving.update(300); // 600
    moving.update(300); // 900
    EXPECT_CALL(mBasicLights[staircase::IBasicLight::kLightNum - 1 - 2],
                turnOn(500))
        .Times(Exactly(1));
    moving.update(300); // 1200
    EXPECT_CALL(mBasicLights[staircase::IBasicLight::kLightNum - 1 - 2],
                turnOn(_))
        .Times(Exactly(0));
    moving.update(300);  // 1500
    moving.update(300);  // 1800
    moving.update(4000); // 5800
}

TEST_F(MovingTests, LightTurningOnLightThree) {
    staircase::Moving moving{
        staircase::IBasicLight::BasicLights{std::begin(mBasicLightPointers),
                                            std::end(mBasicLightPointers)},
        staircase::Moving::Direction::UP, 12000};

    InSequence s;

    moving.update(300); // 300
    moving.update(300); // 600
    moving.update(300); // 900
    moving.update(300); // 1200
    EXPECT_CALL(mBasicLights[3],
                turnOn(12000 / (staircase::IBasicLight::kLightNum + 1)))
        .Times(Exactly(1));
    moving.update(300); // 1500
    EXPECT_CALL(mBasicLights[3], turnOn(_)).Times(Exactly(0));
    moving.update(300);  // 1800
    moving.update(300);  // 2100
    moving.update(4000); // 6100
}

TEST_F(MovingTests, LightTurningOnLightFour) {
    staircase::Moving moving{staircase::IBasicLight::BasicLights{
                                 std::begin(mBasicLightReversePointers),
                                 std::end(mBasicLightReversePointers)},
                             staircase::Moving::Direction::UP, 12000};

    InSequence s;

    moving.update(300); // 300
    moving.update(300); // 600
    moving.update(300); // 900
    moving.update(300); // 1200
    moving.update(300); // 1500
    moving.update(300); // 1800
    moving.update(300); // 2100
    moving.update(300); // 2400
    moving.update(300); // 2700
    EXPECT_CALL(mBasicLights[staircase::IBasicLight::kLightNum - 1 - 4],
                turnOn(12000 / (staircase::IBasicLight::kLightNum + 1)))
        .Times(Exactly(1));
    moving.update(300); // 3000
    EXPECT_CALL(mBasicLights[staircase::IBasicLight::kLightNum - 1 - 4],
                turnOn(_))
        .Times(Exactly(0));
    moving.update(300);  // 3300
    moving.update(300);  // 3600
    moving.update(4000); // 7600
}

TEST_F(MovingTests, LightTurningOnLightFive) {
    staircase::Moving moving{
        staircase::IBasicLight::BasicLights{std::begin(mBasicLightPointers),
                                            std::end(mBasicLightPointers)},
        staircase::Moving::Direction::DOWN, 12000};

    InSequence s;

    moving.update(300); // 300
    moving.update(300); // 600
    moving.update(300); // 900
    moving.update(300); // 1200
    moving.update(300); // 1500
    moving.update(300); // 1800
    moving.update(300); // 2100
    moving.update(300); // 2400
    moving.update(300); // 2700
    moving.update(300); // 3000
    moving.update(300); // 3300
    moving.update(300); // 3600
    moving.update(300); // 3900
    EXPECT_CALL(mBasicLights[5],
                turnOn(12000 / (staircase::IBasicLight::kLightNum + 1)))
        .Times(Exactly(1));
    moving.update(300); // 4200
    EXPECT_CALL(mBasicLights[5], turnOn(_)).Times(Exactly(0));
    moving.update(300);  // 4500
    moving.update(300);  // 4800
    moving.update(4000); // 8800
}

// TODO: check how to calculate actual needed time

} // namespace tests