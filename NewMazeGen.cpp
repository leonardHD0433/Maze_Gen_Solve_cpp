#include <iostream>
#include <random>
#include <algorithm>
using namespace std;

struct Cell {
    int ident = 0;
    bool isvisited = false;
};

// Structure to store values for move direction
struct CellPtr {
    int heightVal=0;
    int widthVal=0;
};

void promptMazeSize(int &height, int &width) {
    cout << "Enter the height of the maze: ";
    cin >> height;
    cout << "Enter the width of the maze: ";
    cin >> width;

    if(height % 2 == 0) 
        height++;
    
    if(width % 2 == 0) 
        width++;
}

void createMaze(Cell **maze, int height, int width) {

    // Block all cells
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            maze[j][i].ident = 1;
        }
    }

    // Set corner values to corner
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

void pickRandomCell(Cell **maze, int height, int width, CellPtr frontier[]) {
    
    int i, j;
    while(true) {
        i = rand() % (height-2);
        j = rand() % (width-2);
        if(maze[i][j].ident != 5) 
            break;
        
    }

    maze[i][j].ident = 0; // Mark the cell as a passage
    frontier[0].heightVal = i;
    frontier[0].widthVal = j;
}

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
                case 3: cout << "E|"; break; // End
                case 4: cout << "*|"; break; // Path
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

    bool nextIteration = true;
    int pathHit = 0;
    CellPtr nextCell;
    
    while (pathHit < 300) {

        for (int i = 0; i < frontierSize; i++) {
            
            CellPtr moveDirection[4] = {{2,0},{-2,0},{0,2},{0,-2}};
            shuffle(begin(moveDirection), end(moveDirection), mt19937{random_device{}()});

            nextCell = frontier[i];
            nextCell.heightVal += moveDirection[0].heightVal;
            nextCell.widthVal += moveDirection[0].widthVal;

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

    createMaze(maze, height, width);
    displayMaze(maze, height, width);

    srand(time(0)); // Seed the random number generator
    pickRandomCell(maze, height, width, frontier);
    frontierSize++;
    displayMaze(maze, height, width);

    createPath(maze, frontier, frontierSize, height, width);
    maze[frontier[0].heightVal][frontier[0].widthVal].ident = 2;
     maze[frontier[1].heightVal][frontier[1].widthVal].ident = 3;


    displayMaze(maze, height, width);
}