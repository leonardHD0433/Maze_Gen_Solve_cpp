#ifndef OUR_OWN_STACK_H
#define OUR_OWN_STACK_H

#include <iostream>
#include <chrono>
#include <thread>
#include <algorithm>
#include <random>
#include "maze_struct.h"
using namespace std;
using namespace chrono;
using namespace this_thread;
using namespace maze_struct;

//UTILITY FUNCTIONS
// Function to clear the screen
void clearScreen();

// Function to delay the display
void delay(int milli);

//STACK FUNCTIONS
// Function to push to the stack
void stackPush(Hist *&TOP, int newHeight, int newWidth);

// Function to count the number of elements in the stack
int stackCount(Hist *TOP);

// Function to pop from the stack
void stackPop(Hist *&TOP);

//MAZE FUNCTIONS
// Function to prompt user for maze size (NEW)
void promptMazeSize(int &HEIGHT, int &WIDTH);

// Function to initialise maze cell values (NEW)
void createMaze(Cell **maze, int HEIGHT, int WIDTH);

// Function to display the maze (MODIFIED)
void displayMaze(Cell **maze, int HEIGHT, int WIDTH);

//show maze generation
void showMazeGen(Cell **maze, int HEIGHT, int WIDTH);

// Function to pick a random cell to start the maze (NEW)
void pickFirstCell(Cell **maze, CellPtr *frontier, int HEIGHT, int WIDTH);

// Function to generate a maze using Prim's algorithm (NEW)
void createPath(Cell **maze, CellPtr *frontier, int &frontierSize, int HEIGHT, int WIDTH);

// Function to check if a move is valid (MODIFIED)
bool isValidMove(Cell **maze, int newHeight, int newWidth);

// Function to solve the maze using DFS (MODIFIED)
bool solveMaze(Hist *&TOP, Cell **maze, int currentHeight, int currentWidth, int numAttempts, int HEIGHT, int WIDTH);

//overloaded function to display the maze after each forward move
void betterMazeDisplay(Cell **maze, int numAttempts, int HEIGHT, int WIDTH);

//overloaded function to display the maze after failed attempts
void betterMazeDisplay(Cell **maze, int height, int width, int numAttempts, int HEIGHT, int WIDTH);
#endif