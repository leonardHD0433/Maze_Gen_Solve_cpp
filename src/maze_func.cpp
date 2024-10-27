#include "../header/maze_func.h"

//UTILITY FUNCTIONS
// Function to clear the screen
void clearScreen() 
{
    cout << "\033[2J\033[1;1H";
}

// Function to delay the display
void delay(int milli)
{
    sleep_for(milliseconds(milli));    
}

//STACK FUNCTIONS
// Function to push to the stack
void stackPush(Hist *&TOP, int newHeight, int newWidth) 
{
    Hist* path= new Hist;
    path->height = newHeight;
    path->width = newWidth;
    path->prev = TOP;
    TOP = path;
}

// Function to count the number of elements in the stack
int stackCount(Hist *TOP) 
{
    int count = 0;
    Hist* temp = TOP;
    while(temp != NULL) 
    {
        count++;
        temp = temp->prev;
    }
    return count;
}

// Function to pop from the stack
void stackPop(Hist *&TOP) 
{
    //Do nothing if stack is empty
    if(TOP == NULL) 
    {
        return;
    }

    //Reassign TOP and delete the previous top for memory management
    Hist* temp = TOP;
    TOP = TOP->prev;
    delete temp;
}

//MAZE FUNCTIONS
// Function to prompt user for maze size (NEW)
void promptMazeSize(int &HEIGHT, int &WIDTH) {
    //odd cuz maze is visually better 
    cout << "Enter an odd value for the height of the maze : ";
    cin >> HEIGHT;

    if(HEIGHT % 2 == 0) 
        cout << "Oopsie, you typed an even num. Lemme fix that for you." << endl;
        delay(300);
        HEIGHT++;
    
    cout << "Enter an odd value for the width of the maze : ";
    cin >> WIDTH;

    if(WIDTH % 2 == 0) 
        cout << "Oopsie, you typed an even num. Lemme fix that for you." << endl;
        delay(300);
        WIDTH++;
}

// Function to initialise maze cell values (NEW)
void createMaze(Cell **maze, int HEIGHT, int WIDTH) {

    // Block all cells
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            maze[i][j].ident = 1;
        }
    }
    showMazeGen(maze, HEIGHT, WIDTH);

    // Set corner values to corner variable
    for (int i = 0; i < WIDTH; i++) {
        maze[0][i].ident = 5;
        maze[1][i].ident = 5;
        maze[HEIGHT - 1][i].ident = 5;
        maze[HEIGHT - 2][i].ident = 5;
    }
    showMazeGen(maze, HEIGHT, WIDTH);

    for (int i = 0; i < HEIGHT; i++) {
        maze[i][0].ident = 5;
        maze[i][1].ident = 5;
        maze[i][WIDTH - 1].ident = 5;
        maze[i][WIDTH - 2].ident = 5;
    }
    showMazeGen(maze, HEIGHT, WIDTH);
}

// Function to display the maze (MODIFIED)
void displayMaze(Cell **maze, int HEIGHT, int WIDTH)
{
    for (int i = 0; i < WIDTH * 2 - 3; i++) {
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
    for (int i = 0; i < WIDTH * 2 - 3; i++) {
        cout << "‾";
    }
    cout << endl;
}

// Function to show maze generation
void showMazeGen(Cell **maze, int HEIGHT, int WIDTH)
{
    clearScreen();
    displayMaze(maze, HEIGHT, WIDTH);
    delay(1);
}

// Function to pick a random cell to start the maze (NEW)
void pickFirstCell(Cell **maze, CellPtr *frontier, int HEIGHT, int WIDTH)
{
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

// Function to generate a maze using Prim's algorithm (NEW)
void createPath(Cell **maze, CellPtr *frontier, int &frontierSize, int HEIGHT, int WIDTH)
{
    //variable to keep track of the number of times the path hits a corner, cell to hold the next cell to move to, and an array to hold the directions to move to.
    int pathHit = 0;
    CellPtr nextCell;
    CellPtr moveDirection[4] = {{2,0},{-2,0},{0,2},{0,-2}};

    //Limit the times the path hits the corner to end program
    while (pathHit < (2.5*HEIGHT*WIDTH)) 
    {

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

            if(frontierSize % 10 == 0)
            {
                showMazeGen(maze, HEIGHT, WIDTH);
            }
        }
        //shuffle the frontier array
        shuffle(frontier, frontier + frontierSize, mt19937{random_device{}()});

        if(pathHit % 10 == 0)
        {
            showMazeGen(maze, HEIGHT, WIDTH);
        }
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
        if(i % 2 != 0)
        {
            showMazeGen(maze, HEIGHT, WIDTH);
        }
    }
}

//overloaded function to display the maze after each forward move
void betterMazeDisplay(Cell **maze, int numAttempts, int HEIGHT, int WIDTH)
{
    clearScreen();
    cout<<"_________________________________________________________________________________________"<<endl;
    cout<<"Attempt: " << numAttempts << endl << endl <<  endl << "Moving Forward..." << endl;
    displayMaze(maze, HEIGHT, WIDTH); 
    delay(50); 
}

//overloaded function to display the maze after failed attempts
void betterMazeDisplay(Cell **maze, int height, int width, int numAttempts, int HEIGHT, int WIDTH)
{
    clearScreen();
    cout << "_________________________________________________________________________________________"<<endl;
    cout << "Attempt: " << numAttempts << endl << endl;
    cout << "Failed Attempt: (" << height << ", " << width << ")" << endl;
    cout << "Backtracking..."<<endl;  
    displayMaze(maze, HEIGHT, WIDTH); 
    delay(50);
}

// Function to check if a move is valid (MODIFIED)
bool isValidMove(Cell **maze, int newHeight, int newWidth) {
    return maze[newHeight][newWidth].ident != 1 && maze[newHeight][newWidth].ident != 5 && !maze[newHeight][newWidth].isvisited ;
}

// Function to solve the maze using DFS (MODIFIED)
bool solveMaze(Hist *&TOP, Cell **maze, int currentHeight, int currentWidth, int numAttempts, int HEIGHT, int WIDTH) 
{
    // If we reached the end
    if (maze[currentHeight][currentWidth].ident == 3) 
    { // 1,1 false
        return true;
    }

    // Mark the current cell as visited
    maze[currentHeight][currentWidth].isvisited = true;
    //if(currentHeight!=1||currentWidth!=1) {betterMazeDisplay(maze, numAttempts);} //display the maze after each forward move except the first move
    

    //Create a move direction array to shuffle the directions
    CellPtr moveDirection[4] = {{1,0},{-1,0},{0,1},{0,-1}};
    shuffle(begin(moveDirection), end(moveDirection), mt19937{random_device{}()});

    // Loop through the move directions
    for (int i = 0; i < 4; i++) 
    {

        int newHeight = currentHeight + moveDirection[i].heightVal;
        int newWidth = currentWidth + moveDirection[i].widthVal;

        if (isValidMove(maze, newHeight, newWidth)) 
        {
            // Mark the new cell to be shown as part of the path if it is not end or start
            if(maze[newHeight][newWidth].ident != 2 && maze[newHeight][newWidth].ident != 3) 
            {
                maze[newHeight][newWidth].ident = 4; 
                betterMazeDisplay(maze, numAttempts, HEIGHT, WIDTH);
            }

            stackPush(TOP, newHeight, newWidth);

            if (solveMaze(TOP, maze,newHeight, newWidth, numAttempts, HEIGHT, WIDTH)) 
            {
                return true;
            }

            // Displays valid but failed attempts  
            maze[newHeight][newWidth].ident = 0;//reset the cell to exclude it from the path
            betterMazeDisplay(maze, newHeight, newWidth, numAttempts, HEIGHT, WIDTH); //display the maze after backtracking
            stackPop(TOP);
        } 
    }

    return false;
}

