// By: Gonçalo Leão

#include "exercises.h"

bool sum3(unsigned int T, unsigned int nums[3]) {
    for (unsigned int a = 1; a < T; ++a) {
        for (unsigned int b = 1; b < T; ++b) {
            unsigned c = T - a - b;
            if (c != 0 && a + b + c == T) {
                nums[0] = a;
                nums[1] = b;
                nums[2] = c;
                return true;
            }
        }
    }
    return false;
}

bool sum3_a(unsigned int T, unsigned int nums[3]) {
    for (unsigned a = 1; a < T; ++a) {
        for (unsigned b = 1; b < T; ++b) {
            for (int c = 1; c < T; ++c) {
                if (c != 0 && a + b + c == T) {
                    nums[0] = a;
                    nums[1] = b;
                    nums[2] = c;
                    return true;
                }
            }
        }
    }
    return false;
}

/// TESTS ///
#include <gtest/gtest.h>

void compareSums(unsigned int selected[3], unsigned int expectedSum) {
    EXPECT_EQ(selected[0] + selected[1] + selected[2], expectedSum);
}

TEST(TP1_Ex1, 3sumExists) {
    unsigned int selected[3];

    unsigned int T = 10;
    EXPECT_EQ(sum3(T,selected), true);
    compareSums(selected,T);

    T = 18;
    EXPECT_EQ(sum3(T,selected), true);
    compareSums(selected,T);
}

TEST(TP1_Ex1, 3sumNotExists) {
    unsigned int selected[3];

    unsigned int T = 1;
    EXPECT_EQ(sum3_a(T,selected), false);
}