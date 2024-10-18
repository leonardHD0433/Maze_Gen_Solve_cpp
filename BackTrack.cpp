// MAJOR FLAW: The algorithm only displays the FIRST PATH it finds, NOT THE SHORTEST PATH.
// Proof at ** Test Maze 2 ** in createMaze function, algo took a 12 tile path instead of an 8 tile path.

/***************************************************************************************
*    Title: Measure execution time of a function in C++ source code
*    Author: Mahapatra S.
*    Date: 2024
*    Code version: -
*    Availability: https://www.geeksforgeeks.org/measure-execution-time-function-cpp/
*
***************************************************************************************/
#include <chrono>
using namespace std::chrono;

#include <iostream>
#include <stack>

using namespace std;

// Structure for maze cell
struct Cell {
    int ident = 0;
    bool isvisited = false;
};

// Structure for stack history tracking
struct Hist {
    int height;
    int width;
};

// Function to initialize the maze
void createMaze(Cell maze[][7], int width, int height, int &currentHeight, int &currentWidth) {
    // Set corner values to blocked
    for (int i = 0; i < width; i++) {
        maze[0][i].ident = 1;
        maze[height - 1][i].ident = 1;
    }

    for (int i = 0; i < height; i++) {
        maze[i][0].ident = 1;
        maze[i][width - 1].ident = 1;
    }

    // Declare the state of the tiles: Blocked(1), Start (2) and End (3)
    maze[currentHeight][currentWidth].ident = 2;
    maze[5][5].ident = 3;

    // ** Test Maze 1 **
    // maze[2][1].ident = 1;
    // maze[2][3].ident = 1;
    // maze[2][4].ident = 1;
    // maze[4][2].ident = 1;
    // maze[4][4].ident = 1;
    // maze[4][5].ident = 1;
    // maze[3][2].ident = 1;

    // ** Test Maze 2 **
    maze[2][2].ident = 1;
    maze[2][3].ident = 1;
    maze[2][4].ident = 1;
    maze[3][2].ident = 1;
    maze[4][2].ident = 1;
    maze[4][4].ident = 1;
    maze[5][4].ident = 1;
}

// Function to display the maze
void displayMaze(Cell maze[][7], int width, int height) {
    for (int i = 0; i < width * 2 + 1; i++) {
        cout << "_";
    }
    cout << endl;
    for (int i = 0; i < height; i++) {
        cout << "|";
        for (int j = 0; j < width; j++) {
            switch (maze[i][j].ident) { 
                // Displays maze according to the state of the tiles
                case 1: cout << "X|"; break; // Blocked
                case 2: cout << "S|"; break; // Start
                case 3: cout << "E|"; break; // End
                case 4: cout << "*|"; break; // Path
                default: cout << " |"; break;
            }
        }
        cout << endl;
    }
    for (int i = 0; i < width * 2 + 1; i++) {
        cout << "‾";
    }
    cout << endl;
}

// Function to check if a move is valid
bool isValidMove(Cell maze[][7], int width, int height, int newHeight, int newWidth) {
    return newHeight >= 0 && newHeight < height && newWidth >= 0 && newWidth < width && maze[newHeight][newWidth].ident != 1 && !maze[newHeight][newWidth].isvisited;
}

// Function to solve the maze using DFS
bool solveMaze(Cell maze[][7], int width, int height, int currentHeight, int currentWidth, stack<Hist> &history) {
    // If we reached the end
    if (maze[currentHeight][currentWidth].ident == 3) { // 1,1 false
        return true;
    }

    // Mark the current cell as visited
    maze[currentHeight][currentWidth].isvisited = true;

    // Possible moves: down, up, right, left
    // It first goes down, then up, then right, then left
    int moveHeight[] = {1, -1, 0, 0};
    int moveWidth[] = {0, 0, 1, -1};

    for (int i = 0; i < 4; i++) {
        int newHeight = currentHeight + moveHeight[i];
        int newWidth = currentWidth + moveWidth[i];

        if (isValidMove(maze, width, height, newHeight, newWidth)) {
            history.push({newHeight, newWidth});
            if (solveMaze(maze, width, height, newHeight, newWidth, history)) {
                return true;
            }
            // Displays valid but failed attempts
            cout << "Failed Attempt: (" << newHeight << ", " << newWidth << ")" << endl;
            history.pop();
        } else {
            // Displays invalid but failed attempts
            cout << "Invalid Move: (" << newHeight << ", " << newWidth << ")" << endl;
        }
    }

    // Backtrack: unmark the current cell as visited
    maze[currentHeight][currentWidth].isvisited = false;
    return false;
}

int main() {
    const int width = 7;
    const int height = 7;
    int currentHeight = 1;
    int currentWidth = 1;

    stack<Hist> history;
    Cell maze[height][width];

    // Initialize the maze
    createMaze(maze, width, height, currentHeight, currentWidth);

    // Display the maze
    displayMaze(maze, width, height);

    // Start the timer
    auto start = high_resolution_clock::now();

    // Solve the maze
    if (solveMaze(maze, width, height, currentHeight, currentWidth, history)) {
        cout << "\nMaze Solved!" << endl;
        cout << "Path taken:" << endl;
        while (!history.empty()) { // Display the path taken
            Hist step = history.top();
            history.pop();
            if (maze[step.height][step.width].ident != 3) {
                maze[step.height][step.width].ident = 4;
            }
            cout << "(" << step.height << ", " << step.width << ")" << endl;
        }
    } else {
        
        cout << "Maze has no Solution" << endl;
    }

    // Stop the timer
    auto stop = high_resolution_clock::now();

    // Display the maze with the solution path
    displayMaze(maze, width, height);

    // Display the time taken to traverse the maze
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken to traverse: " << duration.count() << " microseconds" << endl;

    return 0;
}