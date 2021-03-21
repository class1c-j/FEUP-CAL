#include "exercises.h"
#include <cmath>

int maxSubsequenceDPAux(int k, int A[], unsigned int &j, unsigned int &i, int bestSums[], int bestI[], int bestJ[]) {
    if (bestI[k] != -1) {
        i = bestI[k];
        j = bestJ[k];
        return bestSums[k];
    }

    if (k == 0) {
        bestI[k] = 0;
        bestJ[k] = 0;
        return bestSums[k] = A[k];
    } else if (maxSubsequenceDPAux(k - 1, A, i, j, bestSums, bestI, bestJ) >= 0) {
        bestI[k] = bestI[k - 1];
        bestJ[k] = k;
        return bestSums[k] = A[k] + bestSums[k - 1];
    } else {
        bestI[k] = k;
        bestJ[k] = k;
        return bestSums[k] = A[k];
    }
}


int maxSubsequenceDP(int A[], unsigned int n, unsigned int &i, unsigned int &j) {
    if (n == 0) return 0;
    int bestSums[10000];
    int bestI[10000];
    int bestJ[10000];

    for (unsigned int k = 0; k < n; ++k) {
        bestI[k] = -1;
        bestJ[k] = -1;
    }

    int bestSum = maxSubsequenceDPAux(n - 1, A, j, i, bestSums, bestI, bestJ);
    i = bestI[n - 1];
    j = bestJ[n - 1];

    for (unsigned int k = 0; k < n - 1; ++k) {
        if (bestSums[k] > bestSum) {
            bestSum = bestSums[k];
            i = bestI[k];
            j = bestJ[k];
        }
    }
    return bestSum;
}

int maxSubsequenceDPKadane(int A[], unsigned int n, unsigned int &i, unsigned int &j) {
    if (n == 0) return 0;

    int bestSum = A[0];
    int curSum = A[0];
    i = 0;
    j = 0;
    unsigned int curI = 0;
    for (unsigned int k = 1; k < n; ++k) {
        if (curSum >= 0) {
            curSum += A[k];
        } else {
            curI = k;
            curSum = A[k];
        }

        if (curSum > bestSum) {
            bestSum = curSum;
            i = curI;
            j = k;
        }
    }
    return bestSum;
}

int maxSubsequenceBF(int A[], unsigned int n, unsigned int &i, unsigned int &j) {
    int maxSum = 0;
    int currentSum;
    for (unsigned int start = 0; start < n; ++start) {
        currentSum = 0;
        for (unsigned int end = start; end < n; ++end) {
            currentSum += A[end];
            if (currentSum > maxSum) {
                maxSum = currentSum;
                i = start;
                j = end;
            }
        }
    }
    return maxSum;
}

int maxSubsequenceDCRec(int A[], int start, int finish, unsigned int &i, unsigned int &j) {
    // Step 1: Base Case
    if (start == finish) {
        i = start;
        j = finish;
        return A[start];
    }

    // Step 2: Split
    int start2 = (int) round((start + finish) / 2.0);
    int finish1 = start2 - 1;
    unsigned int i1, j1;
    unsigned int i2, j2;
    int bestSum1 = maxSubsequenceDCRec(A, start, finish1, i1, j1);
    int bestSum2 = maxSubsequenceDCRec(A, start2, finish, i2, j2);

    // Step 3: Merge
    int currSum = A[finish1];
    int bestCrossingSum1 = A[finish1];
    int bestCrossingI = finish1;

    for (int k = finish1 - 1; k >= start; k--) {
        currSum += A[k];
        if (currSum > bestCrossingSum1) {
            bestCrossingSum1 = currSum;
            bestCrossingI = k;
        }
    }

    currSum = A[start2];
    int bestCrossingSum2 = A[start2];
    int bestCrossingJ = start2;

    for (int k = start2 + 1; k <= finish; k++) {
        currSum += A[k];
        if (currSum > bestCrossingSum2) {
            bestCrossingSum2 = currSum;
            bestCrossingJ = k;
        }
    }

    int bestCrossingSum = bestCrossingSum1 + bestCrossingSum2;

    if (bestCrossingSum > bestSum1 && bestCrossingSum > bestSum2) {
        i = bestCrossingI;
        j = bestCrossingJ;
        return bestCrossingSum;
    } else if (bestSum1 > bestSum2) {
        i = i1;
        j = j1;
        return bestSum1;
    } else {
        i = i2;
        j = j2;
        return bestSum2;
    }
}

int maxSubsequenceDC(int A[], unsigned int n, unsigned int &i, unsigned int &j) {
    if (n == 0) return 0;
    return maxSubsequenceDCRec(A, 0, n - 1, i, j);
}

#include <chrono>
#include <iostream>


void testPerformanceMaxSubsequence() {
    srand(time(NULL));
    const unsigned long N_ITERATIONS = 5;
    const unsigned long MIN_SIZE = 0;
    const unsigned long MAX_SIZE = 10000;
    const unsigned long STEP_SIZE = 500;
    int A[MAX_SIZE];
    std::cout << "n; DP time (s); DC time (s)" << std::endl;
    for (unsigned long size = MIN_SIZE; size < MAX_SIZE; size += STEP_SIZE) {
        auto elapsedDP = std::chrono::duration<double>::zero();
        auto elapsedDC = std::chrono::duration<double>::zero();
        for (unsigned long k = 0; k < N_ITERATIONS; ++k) {
            for (unsigned long i = 0; i < size; ++i) {
                A[i] = rand() % (10 * size) - (5 * size);
            }

            unsigned i, j;
            auto start = std::chrono::high_resolution_clock::now();
            maxSubsequenceDP(A, size, i, j);
            auto end = std::chrono::high_resolution_clock::now();
            elapsedDP += (end - start);

            start = std::chrono::high_resolution_clock::now();
            maxSubsequenceDC(A, size, i, j);
            end = std::chrono::high_resolution_clock::now();
            elapsedDC += (end - start);
        }
        std::cout << size << ";" << (elapsedDP.count() / N_ITERATIONS) << ";" << (elapsedDC.count() / N_ITERATIONS)
                  << std::endl;
    }
}

/// TESTS ///
#include <gtest/gtest.h>

TEST(TP4_Ex6, testMaxSubsequence) {
    int A1[] = {1, 2, 3, 4};
    unsigned int n1 = 4;
    unsigned int i, j;
    EXPECT_EQ(maxSubsequenceDP(A1, n1, i, j), 10);
    EXPECT_EQ(i, 0);
    EXPECT_EQ(j, 3);

    int A2[] = {2, -1, -3, 4};
    unsigned int n2 = 4;
    EXPECT_EQ(maxSubsequenceDP(A2, n2, i, j), 4);
    EXPECT_EQ(i, 3);
    EXPECT_EQ(j, 3);

    int A3[] = {2, -1, 3, 4};
    unsigned int n3 = 4;
    EXPECT_EQ(maxSubsequenceDP(A3, n3, i, j), 8);
    EXPECT_EQ(i, 0);
    EXPECT_EQ(j, 3);

    int A4[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    unsigned int n4 = 9;
    EXPECT_EQ(maxSubsequenceDP(A4, n4, i, j), 6);
    EXPECT_EQ(i, 3);
    EXPECT_EQ(j, 6);

    testPerformanceMaxSubsequence();
}