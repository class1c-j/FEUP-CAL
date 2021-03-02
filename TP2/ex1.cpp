#include "exercises.h"

Labyrinth::Labyrinth(int values[10][10]) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++)
            labyrinth[i][j] = values[i][j];
    }
    initializeVisited();
}

#include <iostream>

void Labyrinth::print() const {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            std::cout << labyrinth[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

bool Labyrinth::findGoal(int x, int y) {

    if (x < 0 || x > 9 || y < 0 || y > 9 || labyrinth[x][y] == MAZE_WALL || visited[x][y]) return false;
    if (labyrinth[x][y] == MAZE_EXIT) return true;

    visited[x][y] = true;  // mark current as visited

    if (findGoal(x + 1, y)) return true; // try going right
    else if (findGoal(x - 1, y)) return true;  // try going left
    else if (findGoal(x, y + 1)) return true; // try going down
    else if (findGoal(x, y - 1)) return true; // try going up

    return false;
}

void Labyrinth::initializeVisited() {
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            visited[i][j] = false;
}

/// TESTS ///
#include <gtest/gtest.h>

TEST(TP2_Ex1, testLabyrinthPossible) {
    // This test assumes that the mazes only have a possible path to the exit
    int lab1[10][10] = {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 1, 1, 1, 1, 1, 0, 1, 0, 0},
            {0, 1, 0, 0, 0, 1, 0, 1, 0, 0},
            {0, 1, 1, 0, 1, 1, 1, 1, 1, 0},
            {0, 1, 0, 0, 0, 1, 0, 0, 0, 0},
            {0, 1, 0, 1, 0, 1, 1, 1, 1, 0},
            {0, 1, 1, 1, 0, 0, 1, 0, 1, 0},
            {0, 1, 0, 0, 0, 0, 1, 0, 1, 0},
            {0, 1, 1, 1, 0, 0, 1, 2, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

    Labyrinth l1(lab1);
    EXPECT_EQ(l1.findGoal(1, 1), true);
}

TEST(TP2_Ex1, testLabyrinthImpossible) {
    int lab1[10][10] = {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 1, 1, 1, 1, 1, 0, 1, 0, 0},
            {0, 1, 0, 0, 0, 1, 0, 1, 0, 0},
            {0, 1, 1, 0, 1, 1, 1, 1, 1, 0},
            {0, 1, 0, 0, 0, 1, 0, 0, 0, 0},
            {0, 1, 0, 1, 0, 1, 1, 1, 1, 0},
            {0, 1, 1, 1, 0, 0, 1, 0, 1, 0},
            {0, 1, 0, 0, 0, 0, 1, 0, 1, 0},
            {0, 1, 1, 1, 0, 0, 0, 2, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

    Labyrinth l1(lab1);
    EXPECT_EQ(l1.findGoal(1, 1), false);
}