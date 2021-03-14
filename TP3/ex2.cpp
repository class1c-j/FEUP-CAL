#include "exercises.h"
#include <cmath>
int maxSubsequenceDCRec(int A[], int start, int finish, int &i, int &j) {
    // Step 1: Base Case
    if (start == finish) {
        i = start;
        j = finish;
        return A[start];
    }

    // Step 2: Split
    int start2 = (int) round((start + finish) / 2.0);
    int finish1 = start2 - 1;
    int i1, j1;
    int i2, j2;
    int bestSum1 = maxSubsequenceDCRec(A, start, finish1, i1, j1);
    int bestSum2 = maxSubsequenceDCRec(A, start2, finish, i2, j2);

    // Step 3: Merge
    int currSum = A[finish1];
    int bestCrossingSum1 = A[finish1];
    int bestCrossingI = finish1;

    for(int k = finish1 - 1; k >= start; k--) {
        currSum += A[k];
        if(currSum > bestCrossingSum1) {
            bestCrossingSum1 = currSum;
            bestCrossingI = k;
        }
    }

    currSum = A[start2];
    int bestCrossingSum2 = A[start2];
    int bestCrossingJ = start2;

    for(int k = start2 + 1; k <= finish; k++) {
        currSum += A[k];
        if(currSum > bestCrossingSum2) {
            bestCrossingSum2 = currSum;
            bestCrossingJ = k;
        }
    }

    int bestCrossingSum = bestCrossingSum1 + bestCrossingSum2;

    if(bestCrossingSum > bestSum1 && bestCrossingSum > bestSum2){
        i = bestCrossingI;
        j = bestCrossingJ;
        return bestCrossingSum;
    } else if(bestSum1 > bestSum2){
        i = i1;
        j = j1;
        return bestSum1;
    } else {
        i = i2;
        j = j2;
        return bestSum2;
    }
}

int maxSubsequenceDC(int A[], unsigned int n, int &i, int &j) {
    if(n == 0) return 0;
    return maxSubsequenceDCRec(A, 0, n-1, i, j);
}

/// TESTS ///
#include <gtest/gtest.h>

TEST(TP3_Ex2, testMaxSubsequence) {
    int A1[] = {1, 2, 3, 4};
    unsigned int n1 = 4;
    int i, j;
    EXPECT_EQ(maxSubsequenceDC(A1,n1,i,j), 10);
    EXPECT_EQ(i, 0);
    EXPECT_EQ(j, 3);

    int A2[] = {2, -1, -3, 4};
    unsigned int n2 = 4;
    EXPECT_EQ(maxSubsequenceDC(A2,n2,i,j), 4);
    EXPECT_EQ(i, 3);
    EXPECT_EQ(j, 3);

    int A3[] = {2, -1, 3, 4};
    unsigned int n3 = 4;
    EXPECT_EQ(maxSubsequenceDC(A3,n3,i,j), 8);
    EXPECT_EQ(i, 0);
    EXPECT_EQ(j, 3);

    int A4[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    unsigned int n4 = 9;
    EXPECT_EQ(maxSubsequenceDC(A4,n4,i,j), 6);
    EXPECT_EQ(i, 3);
    EXPECT_EQ(j, 6);
}