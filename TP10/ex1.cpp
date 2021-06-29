#include "exercises.h"

#include <vector>
#include <iostream>
#include <fstream>

int kmpMatcher(std::string text, std::string pattern) {
    size_t n = text.length();
    size_t m = pattern.length();
    int res = 0;
    if (m == 0) {
        return 0;
    }
    /* Compute prefix function */
    int piFunction[m];
    if (m > 0) {
        piFunction[0] = -1;
    }
    int k = -1;
    for (int q = 1; q < m; ++q) {
        while (k > -1 && pattern.at(k + 1) != pattern.at(q)) {
            k = piFunction[k];
        }
        if (pattern.at(k + 1) == pattern.at(q)) {
            ++k;
        }
        piFunction[q] = k;
    }
    /* Match pattern */
    int q = -1;
    for (int i = 0; i < n; ++i) {
        while (q > -1 && pattern.at(q + 1) != text.at(i)) {
            q = piFunction[q];
        }
        if (pattern.at(q + 1) == text.at(i)) {
            ++q;
        }
        if (q == m - 1) {
            std::cout << "Pattern occurs with shift " << i - m + 1 << "\n";
            res++;
            q = piFunction[q];
        }
    }
    return res;
}

int numStringMatching(std::string filename, std::string toSearch) {
    std::ifstream file(filename);
    std::string line{};
    int totalCounter = 0;
    for (; std::getline(file, line);) {
        totalCounter += kmpMatcher(line, toSearch);
    }
    return totalCounter;
}

/// TESTS ///
#include <gtest/gtest.h>

TEST(TP10_Ex1, testKmpMatcher) {
    EXPECT_EQ(3, kmpMatcher("aaabaabaacaabaa", "aabaa"));

    EXPECT_EQ(0, kmpMatcher("", "a"));
    EXPECT_EQ(1, kmpMatcher("a", "a"));
}

#define REL_PATH std::string("../TP10/") // relative path to the tests

TEST(TP10_Ex1, testNumStringMatching) {
    int num1 = numStringMatching(REL_PATH + "text1.txt", "estrutura de dados");
    EXPECT_EQ(3, num1);

    int num2 = numStringMatching(REL_PATH + "text2.txt", "estrutura de dados");
    EXPECT_EQ(2, num2);
}