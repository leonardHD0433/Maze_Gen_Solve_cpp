#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

struct Cell {
    int ident = 1;
    bool isvisited = false;
};

void promptMazeSize(int &height, int &width) {
    cout << "Enter the height of the maze: ";
    cin >> height;
    cout << "Enter the width of the maze: ";
    cin >> width;
}

void createMaze(Cell **maze, int height, int width) {
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
    maze[1][1].ident = 2; // Start
    maze[height - 2][width - 2].ident = 3; // End
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
        maze[i][j].ident = 4; // Mark the path
    }

    maze[height - 2][width - 2].ident = 3; // Ensure the end point is correctly marked
}

void generateComplexity(Cell **maze, int height, int width) {
    // Scatter X barriers without affecting the outermost barrier and the path
    for (int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            // Skip cells that are part of the guaranteed path or start/end points
            if (maze[i][j].ident == 2 || maze[i][j].ident == 3 || maze[i][j].ident == 4) {
                continue;
            }
            // Randomly place X barriers
            if (rand() % 2 == 0) { // Adjust the probability as needed
                maze[i][j].ident = 0; // Empty
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
                case 1: cout << "X|"; break; // Blocked
                case 2: cout << "S|"; break; // Start
                case 3: cout << "E|"; break; // End
                case 4: cout << "*|"; break; // Path
                default: cout << " |"; break; // Empty
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
    srand(time(0));
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
    generateComplexity(maze, height, width);
    displayMaze(maze, height, width);

    // Deallocate memory
    for (int i = 0; i < height; i++) {
        delete[] maze[i];
    }
    delete[] maze;

    return 0;
}