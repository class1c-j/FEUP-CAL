#include "exercises.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

int editDistance(std::string pattern, std::string text) {
    size_t n = text.length();
    std::vector<int> dp(n + 1);
    int oldValue, newValue;
    for (int j = 0; j <= n; ++j) {
        dp[j] = j;
    }
    size_t m = pattern.length();
    for (int i = 1; i <= m; ++i) {
        oldValue = dp[0];
        dp[0] = i;
        for (int j = 1; j <= n; ++j) {
            if (pattern.at(i - 1) == text.at(j - 1)) {
                newValue = oldValue;
            } else {
                newValue = 1 + std::min(std::min(oldValue, dp[j]), dp[j - 1]);
            }
            oldValue = dp[j];
            dp[j] = newValue;
        }
    }
    return dp[n];
}

float numApproximateStringMatching(std::string filename, std::string toSearch) {
    std::ifstream file(filename);
    std::string line{};
    float numberOfWords = 0;
    float totalDistance = 0;
    for (; std::getline(file, line);) {
        std::stringstream ss(line);
        std::string word{};
        while (!ss.eof()) {
            ss >> word;
            totalDistance += (float) editDistance(toSearch, word);
            ++numberOfWords;
        }
    }
    return totalDistance / numberOfWords;
}

/// TESTS ///
#include <gtest/gtest.h>

TEST(TP10_Ex2, testEditDistance) {
    EXPECT_EQ(3, editDistance("abcdefghijkl", "bcdeffghixkl"));
    EXPECT_EQ(3, editDistance("bcdeffghixkl", "abcdefghijkl"));

    EXPECT_EQ(1, editDistance("", "b"));
    EXPECT_EQ(1, editDistance("a", ""));
    EXPECT_EQ(1, editDistance("a", "b"));
    EXPECT_EQ(0, editDistance("", ""));
    EXPECT_EQ(0, editDistance("a", "a"));
}

#define REL_PATH std::string("../TP10/") // relative path to the tests

TEST(TP10_Ex2, testNumApproximateStringMatching) {
    const float delta = 0.01;

    const float expected1 = 7.76;
    float dist1 = numApproximateStringMatching(REL_PATH + "text1.txt", "estrutur");
    ASSERT_LE(expected1 - delta, dist1);
    ASSERT_GE(expected1 + delta, dist1);

    const float expected2 = 7.49;
    float dist2 = numApproximateStringMatching(REL_PATH + "text2.txt", "estrutur");
    ASSERT_LE(expected2 - delta, dist2);
    ASSERT_GE(expected2 + delta, dist2);
}