#include "exercises.h"

bool Activity::operator==(const Activity &a2) const {
    return start == a2.start && finish == a2.finish;
}

bool Activity::operator<(const Activity &a2) const {
    return start < a2.start;
}

bool Activity::overrides(const Activity &a2) const {
    return (finish > a2.start && start < a2.start) || (start < a2.finish && finish > a2.finish);
}

std::vector<Activity> earliestFinishScheduling(std::vector<Activity> A) {
    std::sort(A.begin(), A.end());
    std::vector<Activity> result{};
    while (!A.empty()) {
        Activity a = A.front();  // earliest finishing time
        result.push_back(a);
        A.erase(std::remove_if(A.begin(), A.end(), [&](const Activity &activity){
            return activity.overrides(a) || activity == a;
        }), A.end());
    }
    return result;
}

/// TESTS ///
#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(TP1_Ex6, activityScheduling) {
    std::vector<Activity> A = {{10,20}, {30, 35}, {5, 15}, {10, 40}, {40, 50}};
    std::vector<Activity> V = earliestFinishScheduling(A);
    EXPECT_EQ(V.size(), 3 );
    ASSERT_THAT(V,  ::testing::ElementsAre(Activity(5, 15), Activity(30, 35), Activity(40, 50)));
}