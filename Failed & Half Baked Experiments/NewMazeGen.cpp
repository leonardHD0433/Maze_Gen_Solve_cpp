#include <iostream>
#include <random>
#include <algorithm>
using namespace std;

struct Cell {
    int ident = 0;
    bool isvisited = false;
};

// Structure to store values for move direction (same as struct Random)
struct CellPtr {
    int heightVal=0;
    int widthVal=0;
};

// Function to prompt user for maze size
void promptMazeSize(int &height, int &width) {
    cout << "Enter an odd value for the height of the maze : ";
    cin >> height;
    cout << "Enter an odd value for the width of the maze : ";
    cin >> width;

    if(height % 2 == 0) 
        height++;
    
    if(width % 2 == 0) 
        width++;
}

// Function to create the maze
void createMaze(Cell **maze, int height, int width) {

    // Block all cells
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            maze[j][i].ident = 1;
        }
    }

    // Set corner values to corner variable
    for (int i = 0; i < width; i++) {
        maze[0][i].ident = 5;
        maze[1][i].ident = 5;
        maze[height - 1][i].ident = 5;
        maze[height - 2][i].ident = 5;
    }

    for (int i = 0; i < height; i++) {
        maze[i][0].ident = 5;
        maze[i][1].ident = 5;
        maze[i][width - 1].ident = 5;
        maze[i][width - 2].ident = 5;
    }
}

// Function to pick a random cell to start the maze
void pickRandomCell(Cell **maze, int height, int width, CellPtr frontier[]) {
    
    int i, j;
    while(true) {
        i = rand() % (height-2);
        j = rand() % (width-2);

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

// Function to display the maze
void displayMaze(Cell **maze, int height, int width) {
    for (int i = 0; i < width * 2 - 1; i++) {
        cout << "_";
    }
    cout << endl;
    for (int i = 1; i < height-1; i++) {
        cout << "|";
        for (int j = 1; j < width-1; j++) {
            switch (maze[i][j].ident) {
                case 1: cout << "\033[32mX\033[0m|"; break; // Blocked
                case 2: cout << "\033[93mS\033[0m|"; break; // Start
                case 3: cout << "\033[93mE\033[0m|"; break; // End
                case 4: cout << "\033[36m|*\033[0m|"; break; // Path
                case 5: cout << "\033[90m#\033[0m|"; break; // Corner
                default: cout << " |"; break; // Empty
            }
        }
        cout << endl;
    }
    for (int i = 0; i < width * 2 - 1; i++) {
        cout << "‾";
    }
    cout << endl;
}

void createPath(Cell **maze, CellPtr frontier[], int &frontierSize, int height, int width) {

    //variable to keep track of the number of times the path hits a corner, cell to hold the next cell to move to, and an array to hold the directions to move to.
    int pathHit = 0;
    CellPtr nextCell;
    CellPtr moveDirection[4] = {{2,0},{-2,0},{0,2},{0,-2}};

    //Limit the times the path hits the corner to end program
    while (pathHit < (2.5*height*width)) {

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
            else if (maze[nextCell.heightVal][nextCell.widthVal].ident == 1){
                maze[nextCell.heightVal][nextCell.widthVal].ident = 0;
                maze[(nextCell.heightVal + frontier[i].heightVal)/2][(nextCell.widthVal + frontier[i].widthVal)/2].ident = 0;
                frontier[frontierSize] = nextCell;
                frontierSize++;
            }
        }
        //shuffle the frontier array
        shuffle(frontier, frontier + frontierSize, mt19937{random_device{}()});
    }
}

    




int main() {

    int height, width;
    promptMazeSize(height, width);
    
    // Create an array to store all cell frontiers and a size tracker
    CellPtr frontier[height*width];
    int frontierSize = 0;
    
    // Allocate memory for height first
    Cell **maze = new Cell*[height];
    // Allocate memory for width
    for (int i = 0; i < height; i++) {
        maze[i] = new Cell[width];
    }

    //create the maze 
    createMaze(maze, height, width);

    // Seed the random number generator and pick starting cell
    srand(time(0)); 
    pickRandomCell(maze, height, width, frontier);
    frontierSize++;

    // Display the maze before creating the path
    displayMaze(maze, height, width);

    // Create the path
    createPath(maze, frontier, frontierSize, height, width);

    //generate the start and end points
    maze[frontier[0].heightVal][frontier[0].widthVal].ident = 2;
    maze[frontier[1].heightVal][frontier[1].widthVal].ident = 3;

    // Display the maze after creating the path
    displayMaze(maze, height, width);
}