// MAJOR FLAW: The algorithm only displays the FIRST PATH it finds, NOT THE SHORTEST PATH.
// Proof at ** Test Maze 2 ** in createMaze function, algo took a 12 tile path instead of an 8 tile path.
//Stack functions as a single linked list with a TOP variable to keep track of most recent added value.

/***************************************************************************************
*    Title: Measure execution time of a function in C++ source code
*    Author: Mahapatra S.
*    Date: 2024
*    Code version: -
*    Availability: https://www.geeksforgeeks.org/measure-execution-time-function-cpp/
*
***************************************************************************************/
#include <iostream>
#include <random>
#include <algorithm>
#include "../header/maze_func.h"
using namespace std;

//Global variable for stack top and maze dimensions
Hist* TOP = NULL;
const int WIDTH = 7;
const int HEIGHT = 7;

// Function to initialize the maze
void createMaze(Cell maze[][7], int &currentHeight, int &currentWidth) {
    // Set corner values to blocked
    for (int i = 0; i < WIDTH; i++) {
        maze[0][i].ident = 1;
        maze[HEIGHT - 1][i].ident = 1;
    }

    for (int i = 0; i < HEIGHT; i++) {
        maze[i][0].ident = 1;
        maze[i][WIDTH - 1].ident = 1;
    }

    // Declare the state of the tiles: Blocked(1), Start (2) and End (3)
    maze[currentHeight][currentWidth].ident = 2;
    maze[5][5].ident = 3;

    //** Test Maze 1 **
    maze[2][1].ident = 1;
    maze[2][3].ident = 1;
    maze[2][4].ident = 1;
    maze[4][2].ident = 1;
    maze[4][4].ident = 1;
    maze[4][5].ident = 1;
    maze[3][2].ident = 1;

    // // ** Test Maze 2 **
    // maze[2][2].ident = 1;
    // maze[2][3].ident = 1;
    // maze[2][4].ident = 1;
    // maze[3][2].ident = 1;
    // maze[4][2].ident = 1;
    // maze[4][4].ident = 1;
    // maze[5][4].ident = 1;
}

// Function to display the maze
void displayMaze(Cell maze[][7]) {
    for (int i = 0; i < WIDTH * 2 + 1; i++) {
        cout << "_";
    }
    cout << endl;
    for (int i = 0; i < HEIGHT; i++) {
        cout << "|";
        for (int j = 0; j < WIDTH; j++) {
                
            switch (maze[i][j].ident) { 
                // Displays maze according to the state of the tiles
                case 1: cout << "X|"; break; // Blocked
                case 2: cout << "S|"; break; // Start
                case 3: cout << "E|"; break; // End
                case 4: cout << "*|"; break; // Path
                default: cout << " |"; break;// Empty
            }
        }
        cout << endl;
    }
    for (int i = 0; i < WIDTH * 2 + 1; i++) {
        cout << "‾";
    }
    cout << endl;
}

// Function to check if a move is valid
bool isValidMove(Cell maze[][7], int newHeight, int newWidth) {
    return newHeight >= 0 && newHeight < HEIGHT && newWidth >= 0 && newWidth < WIDTH && maze[newHeight][newWidth].ident != 1 && !maze[newHeight][newWidth].isvisited ;
}

//overloaded function to display the maze after each forward move
void betterMazeDisplay(Cell maze[][7], int numAttempts)
{
    clearScreen();
    cout<<"_________________________________________________________________________________________"<<endl;
    cout<<"Attempt: " << numAttempts << endl << endl <<  endl << "Moving Forward..." << endl;
    displayMaze(maze); 
    delay(); 
}

//overloaded function to display the maze after failed attempts
void betterMazeDisplay(Cell maze[][7], int height, int width, int numAttempts)
{
    clearScreen();
    cout << "_________________________________________________________________________________________"<<endl;
    cout << "Attempt: " << numAttempts << endl << endl;
    cout << "Failed Attempt: (" << height << ", " << width << ")" << endl;
    cout << "Backtracking..."<<endl;  
    displayMaze(maze); 
    delay();
}

// Function to solve the maze using DFS
bool solveMaze(Cell maze[][7], int currentHeight, int currentWidth, int numAttempts) {
    // If we reached the end
    if (maze[currentHeight][currentWidth].ident == 3) { // 1,1 false
        return true;
    }

    // Mark the current cell as visited
    maze[currentHeight][currentWidth].isvisited = true;
    if(currentHeight!=1||currentWidth!=1) {betterMazeDisplay(maze, numAttempts);} //display the maze after each forward move except the first move
    

    //Create a move direction array to shuffle the directions
    Random moveDirection[4] = {{1,0},{-1,0},{0,1},{0,-1}};
    shuffle(begin(moveDirection), end(moveDirection), mt19937{random_device{}()});

    // Loop through the move directions
    for (int i = 0; i < 4; i++) {

        int newHeight = currentHeight + moveDirection[i].heightVal;
        int newWidth = currentWidth + moveDirection[i].widthVal;

        if (isValidMove(maze, newHeight, newWidth)) {

            // Mark the new cell to be shown as part of the path if it is not end or start
            if(maze[newHeight][newWidth].ident != 2 && maze[newHeight][newWidth].ident != 3) {
                maze[newHeight][newWidth].ident = 4; 
            }
            stackPush(TOP, newHeight, newWidth);
            if (solveMaze(maze,newHeight, newWidth, numAttempts)) {
                return true;
            }
            // Displays valid but failed attempts  
             maze[newHeight][newWidth].ident = 0;//reset the cell to exclude it from the path
             betterMazeDisplay(maze, newHeight, newWidth, numAttempts); //display the maze after backtracking
             stackPop(TOP);
        } 
    }

    return false;
}

int main() {

    // Variables to initialize and keep track of the current position in the maze
    int currentHeight;
    int currentWidth;
    int shortestPath = 0;

    // Variables to store the shortest path stack and maze layout
    Hist* shortestStack;
    Cell shortestMaze[HEIGHT][WIDTH];

    // Start the timer
    auto start = high_resolution_clock::now();

    // Loop to run the algorithm 5 times to find shortest path
    for(int i = 1; i < 6; i++) 
    {
        // Reset values to initial state, reset maze and stack
        TOP = NULL;
        currentHeight = 1;
        currentWidth = 1;
        Cell maze[HEIGHT][WIDTH];

        // Initialize the maze and display it along with attempt number
        createMaze(maze, currentHeight, currentWidth);

        cout<<"_________________________________________________________________________________________"<<endl;
        cout<<"Attempt: "<<i<<endl<<endl;

        displayMaze(maze);
        
        
        // Solve the maze
        if (solveMaze(maze, currentHeight, currentWidth, i))
        {
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

    clearScreen();
    cout << "\nMaze Solved!" << endl;
    cout << "Shortest path taken:" << endl<<endl;

    //assign TOP to shortestStack to display the shortest path taken
    TOP = shortestStack;
    while (TOP!=NULL) { // Display the path taken

        Hist* step = TOP;
        cout << "(" << step->height << ", " << step->width << ")" << endl;
        stackPop(TOP);
    }

    // Display the maze with the solution path
    displayMaze(shortestMaze);

        
    // Stop the timer
    auto stop = high_resolution_clock::now();

    // Display the time taken to traverse the maze
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken to traverse: " << duration.count() << " microseconds" << endl;

    return 0;
} 