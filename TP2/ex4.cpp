#include "exercises.h"

bool Activity::operator==(const Activity &a2) const {
    return start == a2.start && finish == a2.finish;
}

bool Activity::overlaps(const Activity &a2) const {
    return (start < a2.finish) && (a2.start < finish);
}

void activitySelectionBacktrackingRec(std::vector<Activity> &A, std::vector<Activity> &currentSolution,
                                      std::vector<Activity> &bestSolution, int index) {
    if (index == A.size()) {
        if (currentSolution.size() > bestSolution.size()) {
            bestSolution = currentSolution;
        }
    } else {
        bool overlaps = false;
        for (const auto &i : currentSolution) {
            if (A[index].overlaps(i)) {
                overlaps = true;
                break;
            }
        }
        if (!overlaps) currentSolution.push_back(A[index]);
        activitySelectionBacktrackingRec(A, currentSolution, bestSolution, index + 1);
    }
}

std::vector<Activity> activitySelectionBacktracking(std::vector<Activity> A) {
    std::vector<Activity> solution{}, current{};
    activitySelectionBacktrackingRec(A, current, solution, 0);
    return solution;
}

/// TESTS ///
#include <gtest/gtest.h>
#include <gmock/gmock.h>

bool noOverlaps(const std::vector<Activity> &acts) {
    for(unsigned int i = 0; i < acts.size(); i++) {
        Activity A1 = acts.at(i);
        for(unsigned int j = i + 1; j < acts.size(); j++) {
            Activity A2 = acts.at(j);
            if(A1.overlaps(A2)) return false;
        }
    }
    return true;
}

TEST(TP2_Ex4, activityScheduling) {
    std::vector<Activity> A = {{10,20}, {30, 35}, {5, 15}, {10, 40}, {40, 50}};
    std::vector<Activity> V = activitySelectionBacktracking(A);
    EXPECT_EQ(V.size(), 3 );
    EXPECT_EQ(noOverlaps(V), true);
}