#include "exercises.h"

bool isCanonical(unsigned int C[], unsigned int n) {
    //TODO
    unsigned int stock[n];
    unsigned int min = C[2] + 2;
    unsigned int max = C[n-2] + C[n-1];

    // the counter-example will always be in this interval
    for (unsigned int t = min; t < max; ++ t) {

        for (unsigned j = 0; j < n; ++j) {
            stock[j] = n;
        }

        unsigned int bfSolution[n]; unsigned int bfSum = 0;
        changeMakingBF(C, stock, n, t, bfSolution);
        for (unsigned int i = 0; i < n; ++i) {
            bfSum += bfSolution[i];
        }

        unsigned int greedySolution[n]; unsigned int greedySum = 0;
        changeMakingGreedy(C, stock, n, t, greedySolution);
        for (unsigned int i = 0; i < n; ++i) {
            greedySum += greedySolution[i];
        }

        if (greedySum != bfSum) return false;

    }

    return true;
}


/// TESTS ///
#include <gtest/gtest.h>

TEST(TP1_Ex5, canonicalTrue) {
    unsigned int C[] = {1,2,5};
    //EXPECT_EQ(isCanonical(C,3), true);

    unsigned int C2[] = {1,2,5,10};
    //EXPECT_EQ(isCanonical(C2,4), true);
}

TEST(TP1_Ex5, canonicalFalse) {
    unsigned int C[] = {1,4,5};
    EXPECT_EQ(isCanonical(C,3), false);
}