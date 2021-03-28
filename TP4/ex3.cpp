#include "exercises.h"

bool changeMakingDP(unsigned int C[], unsigned int Stock[], unsigned int n, unsigned int T, unsigned int usedCoins[]) {
    const unsigned int INF_N_COINS = T + 1;
    unsigned int minCoins[1001]; // Assumes T <= 1000
    unsigned int lastCoin[1001]; // Assumes n <= 1000
    unsigned int remainingStock[1001]; // Assumes n <= 1000

    // Step 1 : Initialize the DP table
    minCoins[0] = 0;
    for (unsigned int k = 1; k <= T; ++k) {
        minCoins[k] = INF_N_COINS;
        remainingStock[k] = Stock[0];
    }
    // Step 2 : Compute minCoins(i, k) for i > 0
    for (unsigned int i = 0; i < n; ++i) {
        if (Stock[i] == 0) continue;
        for (unsigned int k = C[i]; k <= T; ++k) {
            if (minCoins[k - C[i]] < minCoins[k]) {
                minCoins[k] = minCoins[k - C[i]] + 1;
                lastCoin[k] = i;
                remainingStock[k] = remainingStock[k - C[i]] - 1;
            }
        }
    }

    if (minCoins[T] == INF_N_COINS) return false;

    // Step 3 : Build the solution
    for (unsigned int i = 0; i < n; ++i) {
        usedCoins[i] = 0;
    }
    int k = T;
    while (k > 0) {
        int nextID = lastCoin[k];
        ++usedCoins[nextID];
        k -= C[nextID];
    }
    return true;
}

/// TESTS ///
#include <gtest/gtest.h>

TEST(TP4_Ex3, hasChangeCanonical) {
    unsigned int C[] = {1, 2, 5, 10};
    unsigned int Stock[] = {1, 1, 1, 1};
    unsigned int n = 4;
    unsigned int usedCoins[4];

    EXPECT_EQ(changeMakingDP(C, Stock, n, 13, usedCoins), true);
    EXPECT_EQ(usedCoins[0], 1);
    EXPECT_EQ(usedCoins[1], 1);
    EXPECT_EQ(usedCoins[2], 0);
    EXPECT_EQ(usedCoins[3], 1);

    unsigned int Stock2[] = {1, 2, 4, 2};
    EXPECT_EQ(changeMakingDP(C, Stock2, n, 38, usedCoins), true);
    EXPECT_EQ(usedCoins[0], 1);
    EXPECT_EQ(usedCoins[1], 1);
    EXPECT_EQ(usedCoins[2], 3);
    EXPECT_EQ(usedCoins[3], 2);

    unsigned int Stock3[] = {10, 2, 4, 2};
    EXPECT_EQ(changeMakingDP(C, Stock3, n, 10, usedCoins), true);
    EXPECT_EQ(usedCoins[0], 0);
    EXPECT_EQ(usedCoins[1], 0);
    EXPECT_EQ(usedCoins[2], 0);
    EXPECT_EQ(usedCoins[3], 1);
}

TEST(TP4_Ex3, hasChangeNonCanonical) {
    unsigned int C[] = {1, 4, 5};
    unsigned int Stock[] = {2, 2, 1};
    unsigned int n = 3;
    unsigned int usedCoins[3];

    EXPECT_EQ(changeMakingDP(C, Stock, n, 6, usedCoins), true);
    EXPECT_EQ(usedCoins[0], 1);
    EXPECT_EQ(usedCoins[1], 0);
    EXPECT_EQ(usedCoins[2], 1);

    EXPECT_EQ(changeMakingDP(C, Stock, n, 8, usedCoins), true);
    EXPECT_EQ(usedCoins[0], 0);
    EXPECT_EQ(usedCoins[1], 2);
    EXPECT_EQ(usedCoins[2], 0);

    unsigned int Stock2[] = {8, 2, 1};
    EXPECT_EQ(changeMakingDP(C, Stock2, n, 7, usedCoins), true);
    EXPECT_EQ(usedCoins[0], 2);
    EXPECT_EQ(usedCoins[1], 0);
    EXPECT_EQ(usedCoins[2], 1);

    EXPECT_EQ(changeMakingDP(C, Stock2, n, 8, usedCoins), true);
    EXPECT_EQ(usedCoins[0], 0);
    EXPECT_EQ(usedCoins[1], 2);
    EXPECT_EQ(usedCoins[2], 0);
}

TEST(TP4_Ex3, hasNoChange) {
    unsigned int C[] = {1, 2, 5, 10};
    unsigned int Stock[] = {0, 1, 1, 1};
    unsigned int n = 4;
    unsigned int usedCoins[4];

    EXPECT_EQ(changeMakingDP(C, Stock, n, 18, usedCoins), false);
    EXPECT_EQ(changeMakingDP(C, Stock, n, 1, usedCoins), false);
}