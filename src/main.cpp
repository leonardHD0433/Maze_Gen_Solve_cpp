//Stack functions as a single linked list with a TOP variable to keep track of most recent added value.

/***************************************************************************************
Reference 1
*    Title: Measure execution time of a function in C++ source code
*    Author: Mahapatra S.
*    Date: 2024
*    Code version: -
*    Availability: https://www.geeksforgeeks.org/measure-execution-time-function-cpp/

Reference 2
*    Title: How to declare a 2D array dynamically in C++ using new operator
*    Author: shah
*    Date: 2022
*    Code version: -
*    Availability: https://www.geeksforgeeks.org/how-to-declare-a-2d-array-dynamically-in-c-using-new-operator/

Reference 3
*    Title: Pass 2D array parameter to functions in C++
*    Author: sulag
*    Date: 2024
*    Code version: -
*    Availability: https://www.geeksforgeeks.org/pass-2d-array-parameter-to-functions-in-cpp/

Reference 4
*    Refer to MazeGenPrim's.cpp for the original code
***************************************************************************************/

#include <iostream>
#include <random>
#include <algorithm>
#include "../header/maze_func.h"
using namespace std;

//Global variable for stack top and maze dimensions
Hist* TOP = NULL;
int HEIGHT = 0;
int WIDTH = 0;

int main() 
{
    // Variables to initialize and keep track of the current position in the maze
    int currentHeight;
    int currentWidth;
    int shortestPath = 0;

    //prompt for maze size
    promptMazeSize(HEIGHT, WIDTH);
    cout << "Height x Width: " << HEIGHT<<"x"<< WIDTH <<endl;

    // Create an array to store all cell frontiers and a size tracker
    CellPtr frontier[HEIGHT * WIDTH]; //new Var
    int frontierSize = 0; //new Var

    // Variables to store the shortest path stack and maze layout
    Hist *shortestStack;

    //Create shortest maze holder
    // Allocate memory for height first
    Cell **shortestMaze = new Cell*[HEIGHT]; //new Var
    // Allocate memory for width
    for (int i = 0; i < HEIGHT; i++) {
        shortestMaze[i] = new Cell[WIDTH];
    }

    //Create maze
    // Allocate memory for height first
    Cell **maze = new Cell*[HEIGHT]; //new Var
    // Allocate memory for width
    for (int i = 0; i < HEIGHT; i++) {
        maze[i] = new Cell[WIDTH];
    }

    //Generate the maze using Prim's algorithm
    //create the maze 
    createMaze(maze, HEIGHT, WIDTH);

    // Seed the random number generator and pick starting cell
    srand(time(0));
    pickFirstCell(maze, frontier, HEIGHT, WIDTH);
    frontierSize++;

    // Display the maze before creating the path
    clearScreen();
    cout << "Picking a random cell to start the maze..." << endl;
    delay(500);
    showMazeGen(maze, HEIGHT, WIDTH);

    // Create the path
    createPath(maze, frontier, frontierSize, HEIGHT, WIDTH);

    //generate the start and end points
    maze[frontier[0].heightVal][frontier[0].widthVal].ident = 2;
    maze[frontier[1].heightVal][frontier[1].widthVal].ident = 3;

    // Display the maze after creating the path
    clearScreen();
    displayMaze(maze, HEIGHT, WIDTH);
    delay(1000);

    // Start the timer
    auto start = high_resolution_clock::now();

        // Loop to run the algorithm 5 times to find shortest path
    for(int i = 1; i < 1000; i++) 
    {
                // Reset values to initial state, reset maze and stack
        TOP = NULL;
        currentHeight = frontier[0].heightVal;
        currentWidth = frontier[0].widthVal;

        //reset all path tiles and isvisited values
        for (int i = 0; i < WIDTH; i++) 
        {
            for (int j = 0; j < HEIGHT; j++) 
            {
                maze[j][i].isvisited = false;
                if (maze[j][i].ident == 4) 
                {
                    maze[j][i].ident = 0;
                }
            }
        }

                // Solve the maze
        if (solveMaze(TOP, maze, currentHeight, currentWidth, i, HEIGHT, WIDTH))
        {
            //displayMaze(maze);
            // Check if the path taken is shorter than the shortest traversed path or if it is the first path
            if (stackCount(TOP) < shortestPath||shortestPath==0) 
            {

                // Store the shortest path and shortest maze layout
                shortestPath = stackCount(TOP);
                shortestStack = TOP;
                for (int i = 0; i < HEIGHT; i++) 
                {
                    for (int j = 0; j < WIDTH; j++) 
                    {
                        shortestMaze[i][j] = maze[i][j];
                    }
                }
            }
        } 
        else 
        {
            // Display if maze has no solution and exit
            cout << "Maze has no Solution" << endl;
            return 0;
        }
    }

    cout << "\nMaze Solved!" << endl;
    cout << "Shortest path taken:" << endl<<endl;

    //assign TOP to shortestStack to display the shortest path taken
    TOP = shortestStack;
    while (TOP!=NULL) 
    { // Display the path taken

        Hist* step = TOP;
        cout << "(" << step->height << ", " << step->width << ")" << endl;
        stackPop(TOP);
    }

    // Display the maze with the solution path
    displayMaze(shortestMaze, HEIGHT, WIDTH);

    // Stop the timer
    auto stop = high_resolution_clock::now();

    // Display the time taken to traverse the maze
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken to traverse: " << duration.count() << " microseconds" << endl;

    return 0;
} 