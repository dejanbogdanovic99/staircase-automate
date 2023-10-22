#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <mocks/BasicLightMock.hxx>

#include <staircase/IBasicLight.hxx>
#include <staircase/Moving.hxx>
#include <util/StaticDequeue.hxx>

#include <algorithm>
#include <deque>

namespace tests {

using ::testing::_;
using ::testing::InSequence;
using ::testing::NiceMock;

TEST(StaticDequeTests, IntInitialization) { util::StaticDequeue<int, 6> deque; }

TEST(StaticDequeTests, IntInitializationSize) {
    util::StaticDequeue<int, 7> deque;

    EXPECT_EQ(deque.size(), 0);
}

TEST(StaticDequeTests, IntInitializationFull) {
    util::StaticDequeue<int, 7> deque;

    EXPECT_FALSE(deque.full());
}

TEST(StaticDequeTests, IntInitializationEmpty) {
    util::StaticDequeue<int, 7> deque;

    EXPECT_TRUE(deque.empty());
}

TEST(StaticDequeTests, IntPushBack) {
    util::StaticDequeue<int, 7> deque;

    InSequence s;

    deque.pushBack(6);
    EXPECT_EQ(deque.back(), 6);
    EXPECT_EQ(deque.front(), 6);
    EXPECT_FALSE(deque.empty());
    EXPECT_FALSE(deque.full());
    EXPECT_EQ(deque.size(), 1);
}

TEST(StaticDequeTests, IntPushBackTwo) {
    util::StaticDequeue<int, 7> deque;

    InSequence s;

    deque.pushBack(6);
    deque.pushBack(7);

    EXPECT_EQ(deque.back(), 7);
    EXPECT_EQ(deque.front(), 6);
    EXPECT_FALSE(deque.empty());
    EXPECT_FALSE(deque.full());
    EXPECT_EQ(deque.size(), 2);
}

TEST(StaticDequeTests, IntPushBackFull) {
    util::StaticDequeue<int, 7> deque;

    InSequence s;

    deque.pushBack(6);
    deque.pushBack(7);
    deque.pushBack(8);
    EXPECT_EQ(deque.back(), 8);
    EXPECT_EQ(deque.front(), 6);
    EXPECT_FALSE(deque.empty());
    EXPECT_FALSE(deque.full());
    EXPECT_EQ(deque.size(), 3);

    deque.pushBack(9);
    EXPECT_EQ(deque.back(), 9);
    EXPECT_EQ(deque.front(), 6);
    EXPECT_FALSE(deque.empty());
    EXPECT_FALSE(deque.full());
    EXPECT_EQ(deque.size(), 4);

    deque.pushBack(10);
    EXPECT_EQ(deque.back(), 10);
    EXPECT_EQ(deque.front(), 6);
    EXPECT_FALSE(deque.empty());
    EXPECT_FALSE(deque.full());
    EXPECT_EQ(deque.size(), 5);

    deque.pushBack(11);
    EXPECT_EQ(deque.back(), 11);
    EXPECT_EQ(deque.front(), 6);
    EXPECT_FALSE(deque.empty());
    EXPECT_FALSE(deque.full());
    EXPECT_EQ(deque.size(), 6);

    deque.pushBack(12);
    EXPECT_EQ(deque.back(), 12);
    EXPECT_EQ(deque.front(), 6);
    EXPECT_FALSE(deque.empty());
    EXPECT_TRUE(deque.full());
    EXPECT_EQ(deque.size(), 7);
}

TEST(StaticDequeTests, IntPushBackAfterFull) {
    util::StaticDequeue<int, 7> deque;

    InSequence s;

    deque.pushBack(6);
    deque.pushBack(7);
    deque.pushBack(8);
    deque.pushBack(6);
    deque.pushBack(7);
    deque.pushBack(8);
    deque.pushBack(6);
    ASSERT_EQ(deque.back(), 6);
    ASSERT_EQ(deque.front(), 6);
    ASSERT_FALSE(deque.empty());
    ASSERT_TRUE(deque.full());
    ASSERT_EQ(deque.size(), 7);

    deque.pushBack(9);
    EXPECT_EQ(deque.back(), 6);
    EXPECT_EQ(deque.front(), 6);
    EXPECT_FALSE(deque.empty());
    EXPECT_TRUE(deque.full());
    EXPECT_EQ(deque.size(), 7);
}

TEST(StaticDequeTests, IntPopFront) {
    util::StaticDequeue<int, 7> deque;

    InSequence s;

    deque.pushBack(6);
    ASSERT_EQ(deque.back(), 6);
    ASSERT_EQ(deque.front(), 6);
    ASSERT_FALSE(deque.empty());
    ASSERT_FALSE(deque.full());
    ASSERT_EQ(deque.size(), 1);

    deque.popFront();
    EXPECT_TRUE(deque.empty());
    EXPECT_FALSE(deque.full());
    EXPECT_EQ(deque.size(), 0);
}

TEST(StaticDequeTests, IntPopFrontTwo) {
    util::StaticDequeue<int, 7> deque;

    InSequence s;

    deque.pushBack(6);
    deque.pushBack(7);
    ASSERT_EQ(deque.back(), 7);
    ASSERT_EQ(deque.front(), 6);
    ASSERT_FALSE(deque.empty());
    ASSERT_FALSE(deque.full());
    ASSERT_EQ(deque.size(), 2);

    deque.popFront();
    EXPECT_EQ(deque.back(), 7);
    EXPECT_EQ(deque.front(), 7);
    EXPECT_FALSE(deque.empty());
    EXPECT_FALSE(deque.full());
    EXPECT_EQ(deque.size(), 1);

    deque.popFront();
    EXPECT_TRUE(deque.empty());
    EXPECT_FALSE(deque.full());
    EXPECT_EQ(deque.size(), 0);
}

TEST(StaticDequeTests, IntPopFrontFull) {
    util::StaticDequeue<int, 7> deque;

    InSequence s;

    deque.pushBack(6);
    deque.pushBack(7);
    deque.pushBack(8);
    deque.pushBack(9);
    deque.pushBack(10);
    deque.pushBack(11);
    deque.pushBack(12);
    ASSERT_EQ(deque.back(), 12);
    ASSERT_EQ(deque.front(), 6);
    ASSERT_FALSE(deque.empty());
    ASSERT_TRUE(deque.full());
    ASSERT_EQ(deque.size(), 7);

    deque.popFront();
    EXPECT_EQ(deque.back(), 12);
    EXPECT_EQ(deque.front(), 7);
    EXPECT_FALSE(deque.empty());
    EXPECT_FALSE(deque.full());
    EXPECT_EQ(deque.size(), 6);

    deque.popFront();
    EXPECT_EQ(deque.back(), 12);
    EXPECT_EQ(deque.front(), 8);
    EXPECT_FALSE(deque.empty());
    EXPECT_FALSE(deque.full());
    EXPECT_EQ(deque.size(), 5);

    deque.popFront();
    EXPECT_EQ(deque.back(), 12);
    EXPECT_EQ(deque.front(), 9);
    EXPECT_FALSE(deque.empty());
    EXPECT_FALSE(deque.full());
    EXPECT_EQ(deque.size(), 4);

    deque.popFront();
    EXPECT_EQ(deque.back(), 12);
    EXPECT_EQ(deque.front(), 10);
    EXPECT_FALSE(deque.empty());
    EXPECT_FALSE(deque.full());
    EXPECT_EQ(deque.size(), 3);

    deque.popFront();
    EXPECT_EQ(deque.back(), 12);
    EXPECT_EQ(deque.front(), 11);
    EXPECT_FALSE(deque.empty());
    EXPECT_FALSE(deque.full());
    EXPECT_EQ(deque.size(), 2);

    deque.popFront();
    EXPECT_EQ(deque.back(), 12);
    EXPECT_EQ(deque.front(), 12);
    EXPECT_FALSE(deque.empty());
    EXPECT_FALSE(deque.full());
    EXPECT_EQ(deque.size(), 1);

    deque.popFront();
    EXPECT_TRUE(deque.empty());
    EXPECT_FALSE(deque.full());
    EXPECT_EQ(deque.size(), 0);
}

TEST(StaticDequeTests, IntPopFrontEmpty) {
    util::StaticDequeue<int, 7> deque;

    InSequence s;

    ASSERT_TRUE(deque.empty());
    ASSERT_FALSE(deque.full());
    ASSERT_EQ(deque.size(), 0);

    deque.popFront();
    EXPECT_TRUE(deque.empty());
    EXPECT_FALSE(deque.full());
    EXPECT_EQ(deque.size(), 0);
}

TEST(StaticDequeTests, IntCircle) {
    util::StaticDequeue<int, 7> deque;

    InSequence s;

    deque.pushBack(6);
    deque.pushBack(7);
    deque.pushBack(8);
    deque.pushBack(9);
    deque.pushBack(10);
    deque.pushBack(11);
    deque.pushBack(12);
    deque.popFront();
    deque.popFront();
    deque.popFront();
    deque.pushBack(13);
    deque.pushBack(14);
    deque.pushBack(15);
    deque.popFront();

    EXPECT_EQ(deque.back(), 15);
    EXPECT_EQ(deque.front(), 10);
    EXPECT_FALSE(deque.empty());
    EXPECT_FALSE(deque.full());
    EXPECT_EQ(deque.size(), 6);
}

TEST(StaticDequeTests, IntCircle20) {
    util::StaticDequeue<int, 7> deque;

    InSequence s;

    deque.pushBack(7);

    for (int i = 0; i < 20; ++i) {
        deque.pushBack(i);
        deque.pushBack(i + 1);
        deque.popFront();
        deque.popFront();
    }

    EXPECT_EQ(deque.back(), 20);
    EXPECT_EQ(deque.front(), 20);
    EXPECT_FALSE(deque.empty());
    EXPECT_FALSE(deque.full());
    EXPECT_EQ(deque.size(), 1);
}

TEST(StaticDequeTests, IntIterators) {
    util::StaticDequeue<int, 7> deque;

    deque.pushBack(6);
    deque.pushBack(7);
    deque.pushBack(8);

    std::deque<int> stdDeque{6, 7, 8};

    EXPECT_TRUE(std::equal(deque.begin(), deque.end(), std::begin(stdDeque)));
}

TEST(StaticDequeTests, Moving) {
    util::StaticDequeue<staircase::Moving, 7> deque;

    std::array<NiceMock<mocks::BasicLightMock>,
               staircase::IBasicLight::kLightNum>
        lights;
    std::array<staircase::IBasicLight *, staircase::IBasicLight::kLightNum>
        lightPointers;

    std::transform(std::begin(lights), std::end(lights),
                   std::begin(lightPointers),
                   [](auto &basicLight) { return &basicLight; });

    InSequence s;

    deque.pushBack(staircase::Moving{
        staircase::IBasicLight::BasicLights{std::begin(lightPointers),
                                            std::end(lightPointers)},
        staircase::Moving::Direction::DOWN, 12000});

    EXPECT_FALSE(deque.empty());
    EXPECT_FALSE(deque.full());
    EXPECT_EQ(deque.size(), 1);

    deque.pushBack(staircase::Moving{
        staircase::IBasicLight::BasicLights{std::begin(lightPointers),
                                            std::end(lightPointers)},
        staircase::Moving::Direction::DOWN, 11000});

    EXPECT_FALSE(deque.empty());
    EXPECT_FALSE(deque.full());
    EXPECT_EQ(deque.size(), 2);

    deque.popFront();

    EXPECT_FALSE(deque.empty());
    EXPECT_FALSE(deque.full());
    EXPECT_EQ(deque.size(), 1);

    deque.popFront();

    EXPECT_TRUE(deque.empty());
    EXPECT_FALSE(deque.full());
    EXPECT_EQ(deque.size(), 0);
}

} // namespace tests