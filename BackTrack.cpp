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
#include <chrono>
#include <iostream>
using namespace std::chrono;
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
    Hist* prev;
};

//Global variable for stack top
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

// Function to push to the stack
void stackPush(int newHeight, int newWidth) {

    Hist* path= new Hist;
    path->height = newHeight;
    path->width = newWidth;
    path->prev = TOP;
    TOP = path;
}

// Function to pop from the stack
void stackPop() {

    //Do nothing if stack is empty
    if(TOP == NULL) {
        return;
    }

    //Reassign TOP and delete the previous top for memory management
    Hist* temp = TOP;
    TOP = TOP->prev;
    delete temp;
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

            // Displays maze according to the state of the tiles
            if(maze[i][j].ident == 1) {cout << "X|";} // Blocked
            else if(maze[i][j].ident == 2) {cout << "S|";} // Start
            else if(maze[i][j].ident == 3) {cout << "E|";} // End
            else if(maze[i][j].isvisited == true) {cout << "*|";} // Path
            else {cout << " |";} //empty space
                
                
            // switch (maze[i][j].ident) { 
            //     // Displays maze according to the state of the tiles
            //     case 1: cout << "X|"; break; // Blocked
            //     case 2: cout << "S|"; break; // Start
            //     case 3: cout << "E|"; break; // End
            //     case 4: cout << "*|"; break; // Path
            //     default: cout << " |"; break;
            // }
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
    return newHeight >= 0 && newHeight < HEIGHT && newWidth >= 0 && newWidth < WIDTH && maze[newHeight][newWidth].ident != 1 && !maze[newHeight][newWidth].isvisited;
}

// Function to solve the maze using DFS
bool solveMaze(Cell maze[][7], int currentHeight, int currentWidth) {
    // If we reached the end
    if (maze[currentHeight][currentWidth].ident == 3) {
        return true;
    }

    // Mark the current cell as visited
    maze[currentHeight][currentWidth].isvisited = true;
    if(currentHeight!=1||currentWidth!=1) {displayMaze(maze);} //display the maze after each forward move except the first move
    

    // Possible moves: down, up, right, left
    // It first goes down, then up, then right, then left
    int moveHeight[] = {1, -1, 0, 0};
    int moveWidth[] = {0, 0, 1, -1};

    for (int i = 0; i < 4; i++) {
        int newHeight = currentHeight + moveHeight[i];
        int newWidth = currentWidth + moveWidth[i];

        if (isValidMove(maze, newHeight, newWidth)) {
            //displayMaze(maze);
            stackPush(newHeight, newWidth);
            if (solveMaze(maze,newHeight, newWidth)) {
                return true;
            }
            // Displays valid but failed attempts
             cout << "Failed Attempt: (" << newHeight << ", " << newWidth << ")" << endl;
             cout<< "Backtracking..."<<endl;    
             displayMaze(maze); //display the maze after backtracking
             stackPop();
        } 
    }

    // Backtrack: unmark the current cell as visited
    maze[currentHeight][currentWidth].isvisited = false;
    return false;
}


int main() {
    int currentHeight = 1;
    int currentWidth = 1;

    Cell maze[HEIGHT][WIDTH];

    // Initialize the maze and stack
    createMaze(maze, currentHeight, currentWidth);

    // Display the maze
    displayMaze(maze);

    // Start the timer
    auto start = high_resolution_clock::now();

    // Solve the maze
    if (solveMaze(maze, currentHeight, currentWidth)) {
        cout << "\nMaze Solved!" << endl;
        cout << "Path taken:" << endl;
        while (TOP!=NULL) { // Display the path taken
            
            Hist* step = TOP;
            cout << "(" << step->height << ", " << step->width << ")" << endl;
            stackPop();
        }
    } else {
        
        cout << "Maze has no Solution" << endl;
    }

    // Stop the timer
    auto stop = high_resolution_clock::now();

    // Display the maze with the solution path
    displayMaze(maze);

    // Display the time taken to traverse the maze
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken to traverse: " << duration.count() << " microseconds" << endl;

    return 0;
}