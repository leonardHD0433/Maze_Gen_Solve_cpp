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
#include <chrono>
#include <random>
#include <algorithm>
#include <thread>
using namespace std;
using namespace chrono;
using namespace this_thread;

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

// Structure to store values for move direction (CHANGED NAME FRROM RANDOM)
struct CellPtr {
    int heightVal=0;
    int widthVal=0;
};

//Global variable for stack top and maze dimensions
Hist* TOP = NULL;
int WIDTH = 7;
int HEIGHT = 7;


//UTILITY FUNCTIONS
// Function to clear the screen
void clearScreen() 
{
    cout << "\033[2J\033[1;1H";
}

// Function to delay the display
void delay()
{
    sleep_for(milliseconds(1500));    
}

// Function to push to the stack
void stackPush(int newHeight, int newWidth) {

    Hist* path= new Hist;
    path->height = newHeight;
    path->width = newWidth;
    path->prev = TOP;
    TOP = path;
}

// Function to count the number of elements in the stack
int stackCount() {
    int count = 0;
    Hist* temp = TOP;
    while(temp!=NULL) {
        count++;
        temp = temp->prev;
    }
    return count;
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


//MAZE FUNCTIONS
// Function to prompt user for maze size (NEW)
void promptMazeSize() {
    cout << "Enter an odd value for the height of the maze : ";
    cin >> HEIGHT;
    cout << "Enter an odd value for the width of the maze : ";
    cin >> WIDTH;

    if(HEIGHT % 2 == 0) 
        HEIGHT++;
    
    if(WIDTH % 2 == 0) 
        WIDTH++;
}

// Function to initialise maze cell values (NEW)
void createMaze(Cell **maze) {

    // Block all cells
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            maze[i][j].ident = 1;
        }
    }

    // Set corner values to corner variable
    for (int i = 0; i < WIDTH; i++) {
        maze[0][i].ident = 5;
        maze[1][i].ident = 5;
        maze[HEIGHT - 1][i].ident = 5;
        maze[HEIGHT - 2][i].ident = 5;
    }

    for (int i = 0; i < HEIGHT; i++) {
        maze[i][0].ident = 5;
        maze[i][1].ident = 5;
        maze[i][WIDTH - 1].ident = 5;
        maze[i][WIDTH - 2].ident = 5;
    }
}

// Function to pick a random cell to start the maze (NEW)
void pickFirstCell(Cell **maze, CellPtr frontier[]) {
    
    int i, j;
    while(true) {
        i = rand() % (HEIGHT-2);
        j = rand() % (WIDTH-2);

        // Check if the cell is a corner. If not, break the loop
        if(maze[i][j].ident != 5) 
            break;
        
    }

    // Mark the cell as a passage
    maze[i][j].ident = 0; 

    // Add the cell coordinates to the frontier
    frontier[0].heightVal = i;
    frontier[0].widthVal = j;
}

// Function to display the maze (MODIFIED)
void displayMaze(Cell **maze) {
    for (int i = 0; i < WIDTH * 2 - 1; i++) {
        cout << "_";
    }
    cout << endl;
    for (int i = 1; i < HEIGHT-1; i++) {
        cout << "|";
        for (int j = 1; j < WIDTH-1; j++) {
            switch (maze[i][j].ident) {
                case 1: cout << "\033[32mX\033[0m|"; break; // Blocked
                case 2: cout << "\033[93mS\033[0m|"; break; // Start
                case 3: cout << "\033[93mE\033[0m|"; break; // End
                case 4: cout << "\033[91m*\033[0m|"; break; // Path
                case 5: cout << "\033[90m#\033[0m|"; break; // Corner
                default: cout << " |"; break; // Empty
            }
        }
        cout << endl;
    }
    for (int i = 0; i < WIDTH * 2 - 1; i++) {
        cout << "‾";
    }
    cout << endl;
    delay();
}

// Function to generate a maze using Prim's algorithm (NEW)
void createPath(Cell **maze, CellPtr frontier[], int &frontierSize) {

    //variable to keep track of the number of times the path hits a corner, cell to hold the next cell to move to, and an array to hold the directions to move to.
    int pathHit = 0;
    CellPtr nextCell;
    CellPtr moveDirection[4] = {{2,0},{-2,0},{0,2},{0,-2}};

    //Limit the times the path hits the corner to end program
    while (pathHit < (2.5*HEIGHT*WIDTH)) {

        //Loop through the frontier array
        for (int i = 0; i < frontierSize; i++) {
            
            //get a random direction to move to by shuffling the moveDirection array
            shuffle(begin(moveDirection), end(moveDirection), mt19937{random_device{}()});

            //move to the next cell
            nextCell = frontier[i];
            nextCell.heightVal += moveDirection[0].heightVal;
            nextCell.widthVal += moveDirection[0].widthVal;

            //check if the next cell is a corner, a wall, or a path
            if (maze[nextCell.heightVal][nextCell.widthVal].ident == 5) {
                pathHit++;
            }
            else if (maze[nextCell.heightVal][nextCell.widthVal].ident == 1 ) {
                maze[nextCell.heightVal][nextCell.widthVal].ident = 0;
                maze[nextCell.heightVal][nextCell.widthVal].isvisited = true;
                maze[(nextCell.heightVal + frontier[i].heightVal)/2][(nextCell.widthVal + frontier[i].widthVal)/2].ident = 0;
                frontier[frontierSize] = nextCell;
                frontierSize++;
            }
        }
        //shuffle the frontier array
        shuffle(frontier, frontier + frontierSize, mt19937{random_device{}()});
    }

    // delete 2 random blocked cells per line to connect the paths
    for (int i = 1; i < HEIGHT-1; i++) {
        pathHit = 0;
        for (int j = 1; j < WIDTH-1; j++) {
            if (maze[i][j].ident == 1 && rand() % 4 == 1) {
                maze[i][j].ident = 0;
                pathHit++;
                if (pathHit == 2) {
                    break;
                }
            }
        }
    }
}

// Function to check if a move is valid (MODIFIED)
bool isValidMove(Cell **maze, int newHeight, int newWidth) {
    return maze[newHeight][newWidth].ident != 1 && maze[newHeight][newWidth].ident != 5 && !maze[newHeight][newWidth].isvisited ;
}

//overloaded function to display the maze after each forward move
void betterMazeDisplay(Cell **maze, int numAttempts)
{
    clearScreen();
    cout<<"_________________________________________________________________________________________"<<endl;
    cout<<"Attempt: " << numAttempts << endl << endl <<  endl << "Moving Forward..." << endl;
    displayMaze(maze); 
    delay(); 
}

//overloaded function to display the maze after failed attempts
void betterMazeDisplay(Cell **maze, int height, int width, int numAttempts)
{
    clearScreen();
    cout << "_________________________________________________________________________________________"<<endl;
    cout << "Attempt: " << numAttempts << endl << endl;
    cout << "Failed Attempt: (" << height << ", " << width << ")" << endl;
    cout << "Backtracking..."<<endl;  
    displayMaze(maze); 
    delay();
}

// Function to solve the maze using DFS (MODIFIED)
bool solveMaze(Cell **maze, int currentHeight, int currentWidth, int numAttempts) {
    // If we reached the end
    if (maze[currentHeight][currentWidth].ident == 3) { // 1,1 false
        return true;
    }

    // Mark the current cell as visited
    maze[currentHeight][currentWidth].isvisited = true;
    //if(currentHeight!=1||currentWidth!=1) {betterMazeDisplay(maze, numAttempts);} //display the maze after each forward move except the first move
    

    //Create a move direction array to shuffle the directions
    CellPtr moveDirection[4] = {{1,0},{-1,0},{0,1},{0,-1}};
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
            stackPush(newHeight, newWidth);
            if (solveMaze(maze,newHeight, newWidth, numAttempts)) {
                return true;
            }
            // Displays valid but failed attempts  
             maze[newHeight][newWidth].ident = 0;//reset the cell to exclude it from the path
             //betterMazeDisplay(maze, newHeight, newWidth, numAttempts); //display the maze after backtracking
             stackPop();
        } 
    }

    return false;
}


int main() {

    // Variables to initialize and keep track of the current position in the maze
    int currentHeight;
    int currentWidth;
    int shortestPath = 0;

    //prompt for maze size 
    promptMazeSize();
    cout<<HEIGHT<<" "<<WIDTH<<endl;

    // Create an array to store all cell frontiers and a size tracker
    CellPtr frontier[HEIGHT*WIDTH]; //new Var
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
    createMaze(maze);

    // Seed the random number generator and pick starting cell
    srand(time(0)); 
    pickFirstCell(maze, frontier);
    frontierSize++;

    // Display the maze before creating the path
    displayMaze(maze);

    // Create the path
    createPath(maze, frontier, frontierSize);

    //generate the start and end points
    maze[frontier[0].heightVal][frontier[0].widthVal].ident = 2;
    maze[frontier[1].heightVal][frontier[1].widthVal].ident = 3;

    // Display the maze after creating the path
    displayMaze(maze);
    

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
        for (int i = 0; i < WIDTH; i++) {
            for (int j = 0; j < HEIGHT; j++) {
                maze[j][i].isvisited = false;
                if (maze[j][i].ident == 4) {
                    maze[j][i].ident = 0;
                }
            }
        }


        // cout<<"_________________________________________________________________________________________"<<endl;
        // cout<<"Attempt: "<<i<<endl<<endl;

        //displayMaze(maze);
        
        
        // Solve the maze
        if (solveMaze(maze, currentHeight, currentWidth, i))
        {
            //displayMaze(maze);
            // Check if the path taken is shorter than the shortest traversed path or if it is the first path
            if (stackCount() < shortestPath||shortestPath==0) 
            {

                // Store the shortest path and shortest maze layout
                shortestPath = stackCount();
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
    while (TOP!=NULL) { // Display the path taken

        Hist* step = TOP;
        cout << "(" << step->height << ", " << step->width << ")" << endl;
        stackPop();
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