#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

struct Cell {
    int ident = 1;
    bool isvisited = false;
};

struct Stack {
    int x, y;
    Stack* next;
};

void push(Stack*& stack, int x, int y) {
    Stack* newNode = new Stack();
    newNode->x = x;
    newNode->y = y;
    newNode->next = stack;
    stack = newNode;
}

void pop(Stack*& stack, int& x, int& y) {
    if (stack) {
        Stack* temp = stack;
        x = stack->x;
        y = stack->y;
        stack = stack->next;
        delete temp;
    }
}

bool isEmpty(Stack* stack) {
    return stack == nullptr;
}

void promptMazeSize(int &height, int &width) {
    cout << "Enter the height of the maze: ";
    cin >> height;
    cout << "Enter the width of the maze: ";
    cin >> width;
}

void createMaze(Cell **maze, int height, int width) {
    // Declare the state of the tiles: Blocked(1), Start (2) and End (3)
    // Set corner values to blocked
    for (int i = 0; i < width; i++) {
        maze[0][i].ident = 1;
        maze[height - 1][i].ident = 1;
    }

    for (int i = 0; i < height; i++) {
        maze[i][0].ident = 1;
        maze[i][width - 1].ident = 1;
    }
    
    // Set the start and end points
    maze[1][1].ident = 2;
    maze[height - 2][width - 2].ident = 3;
}

void createGuaranteedPath(Cell **maze, int height, int width) {
    int i = 1, j = 1;
    maze[i][j].ident = 2; // Mark the start of the path

    while (i < height - 2 || j < width - 2) {
        if (i < height - 2 && (rand() % 2 == 0 || j == width - 2)) {
            i++;
        } else if (j < width - 2) {
            j++;
        }
        maze[i][j].ident = 5; // Mark the path
    }

    maze[height - 2][width - 2].ident = 3; // Ensure the end point is correctly marked
}

void generateInnerMaze(Cell **maze, int height, int width) {
    srand(time(0)); // Seed for random number generation

    Stack* stack = nullptr;
    push(stack, 1, 1);
    maze[1][1].isvisited = true;

    while (!isEmpty(stack)) {
        int x, y;
        pop(stack, x, y);

        // Get unvisited neighbors
        int neighbors[4][2];
        int count = 0;
        if (x > 1 && !maze[x - 2][y].isvisited) {
            neighbors[count][0] = x - 2;
            neighbors[count][1] = y;
            count++;
        }
        if (x < height - 2 && !maze[x + 2][y].isvisited) {
            neighbors[count][0] = x + 2;
            neighbors[count][1] = y;
            count++;
        }
        if (y > 1 && !maze[x][y - 2].isvisited) {
            neighbors[count][0] = x;
            neighbors[count][1] = y - 2;
            count++;
        }
        if (y < width - 2 && !maze[x][y + 2].isvisited) {
            neighbors[count][0] = x;
            neighbors[count][1] = y + 2;
            count++;
        }

        if (count > 0) {
            // Push the current cell back onto the stack
            push(stack, x, y);

            // Choose a random neighbor
            int r = rand() % count;
            int nx = neighbors[r][0];
            int ny = neighbors[r][1];

            // Remove the wall between the current cell and the chosen cell
            if (maze[nx][ny].ident != 2 && maze[nx][ny].ident != 3 && maze[nx][ny].ident != 5) {
                maze[(x + nx) / 2][(y + ny) / 2].ident = 0;
                maze[nx][ny].ident = 0;
            }

            // Mark the chosen cell as visited and push it onto the stack
            maze[nx][ny].isvisited = true;
            push(stack, nx, ny);
        }
    }

    // Convert the guaranteed path identifier back to the empty path identifier
    for (int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            if (maze[i][j].ident == 5) {
                maze[i][j].ident = 0;
            }
        }
    }
}

// Function to display the maze
void displayMaze(Cell **maze, int height, int width) {
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
                default: cout << " |"; break;// Empty
            }
        }
        cout << endl;
    }
    for (int i = 0; i < width * 2 + 1; i++) {
        cout << "‾";
    }
    cout << endl;
}

int main() {
    int height = 0;
    int width = 0;

    promptMazeSize(height, width);
    cout << "Height: " << height << " Width: " << width << endl;

    // Allocate memory for height first
    Cell **maze = new Cell*[height];
    // Allocate memory for width
    for (int i = 0; i < height; i++) {
        maze[i] = new Cell[width];
    }

    createMaze(maze, height, width);
    createGuaranteedPath(maze, height, width);
    generateInnerMaze(maze, height, width);
    displayMaze(maze, height, width);

    // Free allocated memory
    for (int i = 0; i < height; i++) {
        delete[] maze[i];
    }
    delete[] maze;

    return 0;
}