#ifndef OUR_OWN_STACK_H
#define OUR_OWN_STACK_H

#include <iostream>
#include <chrono>
#include <thread>
#include "maze_struct.h"
using namespace std;
using namespace chrono;
using namespace this_thread;
using namespace maze_struct;

// Function to push to the stack
void stackPush(Hist *&TOP, int newHeight, int newWidth);

// Function to count the number of elements in the stack
int stackCount(Hist *TOP);

// Function to pop from the stack
void stackPop(Hist *&TOP);

// Function to clear the screen
void clearScreen();

// Function to delay the display
void delay();

#endif