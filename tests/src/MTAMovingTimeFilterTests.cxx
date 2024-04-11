#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <staircase/MTAMovingTimeFilter.hxx>

#include <algorithm>
#include <array>

namespace tests {

using ::testing::_;

TEST(MTAMovingTimeFilterTests,
     GivenGetCurrentTimeIsCalledAfterInitializationItReturnsInitialValue) {
    staircase::MTAMovingTimeFilter timeFilter{10000};
    EXPECT_EQ(timeFilter.getCurrentMovingTime(), 10000);
}

TEST(MTAMovingTimeFilterTests,
     GivenProcessNewMovingTimeIsCalledOnceGetCurrentTimeIsImpacted) {
    staircase::MTAMovingTimeFilter timeFilter{10000};
    constexpr hal::Milliseconds expectedValue = (4 * 10000 + 8000) / 5;

    timeFilter.processNewMovingTime(8000);
    EXPECT_EQ(timeFilter.getCurrentMovingTime(), expectedValue);
}

TEST(MTAMovingTimeFilterTests,
     GivenProcessNewMovingTimeIsCalledTwiceGetCurrentTimeIsImpactedMore) {
    staircase::MTAMovingTimeFilter timeFilter{10000};
    constexpr hal::Milliseconds expectedValue = (3 * 10000 + 2 * 8000) / 5;

    timeFilter.processNewMovingTime(8000);
    timeFilter.processNewMovingTime(8000);
    EXPECT_EQ(timeFilter.getCurrentMovingTime(), expectedValue);
}

TEST(MTAMovingTimeFilterTests,
     GivenProcessNewMovingTimeIsCalledThreeTimesGetCurrentTimeIsImpactedMore) {
    staircase::MTAMovingTimeFilter timeFilter{10000};
    constexpr hal::Milliseconds expectedValue = (2 * 10000 + 3 * 8000) / 5;

    timeFilter.processNewMovingTime(8000);
    timeFilter.processNewMovingTime(8000);
    timeFilter.processNewMovingTime(8000);
    EXPECT_EQ(timeFilter.getCurrentMovingTime(), expectedValue);
}

TEST(MTAMovingTimeFilterTests,
     GivenProcessNewMovingTimeIsCalledFourTimesGetCurrentTimeIsImpactedMore) {
    staircase::MTAMovingTimeFilter timeFilter{10000};
    constexpr hal::Milliseconds expectedValue = (10000 + 4 * 8000) / 5;

    timeFilter.processNewMovingTime(8000);
    timeFilter.processNewMovingTime(8000);
    timeFilter.processNewMovingTime(8000);
    timeFilter.processNewMovingTime(8000);
    EXPECT_EQ(timeFilter.getCurrentMovingTime(), expectedValue);
}

TEST(
    MTAMovingTimeFilterTests,
    GivenProcessNewMovingTimeIsCalledFiveTimesGetCurrentTimeHasCompletelyChanged) {
    staircase::MTAMovingTimeFilter timeFilter{10000};
    constexpr hal::Milliseconds expectedValue = 8000;

    timeFilter.processNewMovingTime(8000);
    timeFilter.processNewMovingTime(8000);
    timeFilter.processNewMovingTime(8000);
    timeFilter.processNewMovingTime(8000);
    timeFilter.processNewMovingTime(8000);
    EXPECT_EQ(timeFilter.getCurrentMovingTime(), expectedValue);
}

TEST(MTAMovingTimeFilterTests,
     GivenProcessNewMovingTimeIsCalledResetReturnsToOldValue) {
    staircase::MTAMovingTimeFilter timeFilter{10000};
    constexpr hal::Milliseconds expectedValue = 10000;

    timeFilter.processNewMovingTime(8000);
    timeFilter.reset(expectedValue);
    EXPECT_EQ(timeFilter.getCurrentMovingTime(), expectedValue);
}

} // namespace tests