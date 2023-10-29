#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <mocks/BasicLightMock.hxx>

#include <staircase/IBasicLight.hxx>
#include <staircase/IMoving.hxx>
#include <staircase/Moving.hxx>
#include <staircase/StaticMovingFactory.hxx>

#include <algorithm>
#include <array>

namespace tests {

using ::testing::_;
using ::testing::InSequence;
using ::testing::NiceMock;

class StaticMovingFactoryTests : public ::testing::Test {
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
    std::array<NiceMock<mocks::BasicLightMock>, staircase::kLightNum>
        mBasicLights;
    std::array<staircase::IBasicLight *, staircase::kLightNum>
        mBasicLightPointers;
    std::array<staircase::IBasicLight *, staircase::kLightNum>
        mBasicLightReversePointers;
};

TEST_F(StaticMovingFactoryTests, CreateOne) {
    staircase::StaticMovingFactory<2> factory;

    staircase::IMoving *moving =
        factory.create(staircase::BasicLights{std::begin(mBasicLightPointers),
                                              std::end(mBasicLightPointers)},
                       staircase::IMoving::Direction::UP, 12000);

    ASSERT_NE(moving, nullptr);

    EXPECT_TRUE(moving->isNearBegin());
    EXPECT_FALSE(moving->isNearEnd());
    EXPECT_EQ(moving->getTimePassed(), 0);
    moving->update(12000);
    EXPECT_TRUE(moving->isNearEnd());
    EXPECT_FALSE(moving->isNearBegin());
    EXPECT_EQ(moving->getTimePassed(), 12000);
}

TEST_F(StaticMovingFactoryTests, CreateTwo) {
    staircase::StaticMovingFactory<2> factory;

    staircase::IMoving *moving =
        factory.create(staircase::BasicLights{std::begin(mBasicLightPointers),
                                              std::end(mBasicLightPointers)},
                       staircase::IMoving::Direction::UP, 12000);

    ASSERT_NE(moving, nullptr);

    staircase::IMoving *moving2 =
        factory.create(staircase::BasicLights{std::begin(mBasicLightPointers),
                                              std::end(mBasicLightPointers)},
                       staircase::IMoving::Direction::DOWN, 16000);

    ASSERT_NE(moving2, nullptr);

    EXPECT_TRUE(moving2->isNearBegin());
    EXPECT_FALSE(moving2->isNearEnd());
    EXPECT_EQ(moving2->getTimePassed(), 0);
    moving2->update(16000);
    EXPECT_TRUE(moving2->isNearEnd());
    EXPECT_FALSE(moving2->isNearBegin());
    EXPECT_EQ(moving2->getTimePassed(), 16000);
}

TEST_F(StaticMovingFactoryTests, CreateTwoFailThird) {
    staircase::StaticMovingFactory<2> factory;

    staircase::IMoving *moving =
        factory.create(staircase::BasicLights{std::begin(mBasicLightPointers),
                                              std::end(mBasicLightPointers)},
                       staircase::IMoving::Direction::UP, 12000);

    ASSERT_NE(moving, nullptr);

    staircase::IMoving *moving2 =
        factory.create(staircase::BasicLights{std::begin(mBasicLightPointers),
                                              std::end(mBasicLightPointers)},
                       staircase::IMoving::Direction::DOWN, 16000);

    ASSERT_NE(moving2, nullptr);

    staircase::IMoving *moving3 =
        factory.create(staircase::BasicLights{std::begin(mBasicLightPointers),
                                              std::end(mBasicLightPointers)},
                       staircase::IMoving::Direction::DOWN, 1500);

    EXPECT_EQ(moving3, nullptr);
}

TEST_F(StaticMovingFactoryTests, CreateTwoDestroyOneCreateThirdFailFourth) {
    staircase::StaticMovingFactory<2> factory;

    staircase::IMoving *moving =
        factory.create(staircase::BasicLights{std::begin(mBasicLightPointers),
                                              std::end(mBasicLightPointers)},
                       staircase::IMoving::Direction::UP, 12000);

    ASSERT_NE(moving, nullptr);

    staircase::IMoving *moving2 =
        factory.create(staircase::BasicLights{std::begin(mBasicLightPointers),
                                              std::end(mBasicLightPointers)},
                       staircase::IMoving::Direction::DOWN, 16000);

    ASSERT_NE(moving2, nullptr);

    factory.destroy(moving2);

    staircase::IMoving *moving3 =
        factory.create(staircase::BasicLights{std::begin(mBasicLightPointers),
                                              std::end(mBasicLightPointers)},
                       staircase::IMoving::Direction::DOWN, 19000);

    ASSERT_NE(moving3, nullptr);

    EXPECT_TRUE(moving3->isNearBegin());
    EXPECT_FALSE(moving3->isNearEnd());
    EXPECT_EQ(moving3->getTimePassed(), 0);
    moving3->update(19000);
    EXPECT_TRUE(moving3->isNearEnd());
    EXPECT_FALSE(moving3->isNearBegin());
    EXPECT_EQ(moving3->getTimePassed(), 19000);

    staircase::IMoving *moving4 =
        factory.create(staircase::BasicLights{std::begin(mBasicLightPointers),
                                              std::end(mBasicLightPointers)},
                       staircase::IMoving::Direction::DOWN, 1500);

    EXPECT_EQ(moving4, nullptr);
}

} // namespace tests