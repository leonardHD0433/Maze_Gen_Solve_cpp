#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

struct Cell {
    int ident = 1;
    bool isvisited = false;
};

struct Frontier {
    int x, y;
    Frontier *next;
};

void promptMazeSize(int &height, int &width) {
    cout << "Enter the height of the maze: ";
    cin >> height;
    cout << "Enter the width of the maze: ";
    cin >> width;
}

void createMaze(Cell **maze, int height, int width) {
    // Block all cells
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            maze[j][i].ident = 1;
        }
    }
}

void pickRandomCell(Cell **maze, int height, int width) {
    int i = rand() % height;
    int j = rand() % width;
    
    maze[i][j].ident = 0; // Mark the cell as a passage
}

void calcFrontier(Cell **maze, int height, int width) {
    // Allocate memory for the 2D array dynamically
    int **frontier = new int*[height * width];
    for (int i = 0; i < height * width; i++) {
        frontier[i] = new int[2];
    }

    int **neighbours = new int*[height * width];
    for (int i = 0; i < height * width; i++) {
        neighbours[i] = new int[2];
    }

    int count = 0;
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (maze[i][j].ident == 0) {
                if (i - 2 >= 0 && maze[i - 2][j].ident == 1) {
                    frontier[count][0] = i - 2;
                    frontier[count][1] = j;
                    neighbours[count][0] = i - 1;
                    neighbours[count][1] = j;
                    count++;
                }
                if (i + 2 < height && maze[i + 2][j].ident == 1) {
                    frontier[count][0] = i + 2;
                    frontier[count][1] = j;
                    neighbours[count][0] = i + 1;
                    neighbours[count][1] = j;
                    count++;
                }
                if (j - 2 >= 0 && maze[i][j - 2].ident == 1) {
                    frontier[count][0] = i;
                    frontier[count][1] = j - 2;
                    neighbours[count][0] = i;
                    neighbours[count][1] = j - 1;
                    count++;
                }
                if (j + 2 < width && maze[i][j + 2].ident == 1) {
                    frontier[count][0] = i;
                    frontier[count][1] = j + 2;
                    neighbours[count][0] = i;
                    neighbours[count][1] = j + 1;
                    count++;
                }
            }
        }
    }

    int random = rand() % count;

    // Mark the cell as a passage
    maze[frontier[random][0]][frontier[random][1]].ident = 0; 
    maze[neighbours[random][0]][neighbours[random][1]].ident = 0; 
}

// void calcFrontier(Cell **maze, int height, int width) {
//     Frontier *frontier = new Frontier;
//     if (height - 2 >= 0 && maze[height - 2][width].ident == 1) {
//         frontier->x = height - 2;
//         frontier->y = width;
//         frontier->next = new Frontier;
//         frontier = frontier->next;
//         }
//     if (height + 2 < height && maze[height + 2][width].ident == 1) {
//         frontier[count][0] = i + 2;
//         frontier[count][1] = j;
//         neighbours[count][0] = i + 1;
//         neighbours[count][1] = j;
//         count++;
//         }
//     if (width - 2 >= 0 && maze[height][width - 2].ident == 1) {
//         frontier[count][0] = i;
//         frontier[count][1] = j - 2;
//         neighbours[count][0] = i;
//         neighbours[count][1] = j - 1;
//         count++;
//         }
//     if (width + 2 < width && maze[height][width + 2].ident == 1) {
//         frontier[count][0] = i;
//         frontier[count][1] = j + 2;
//         neighbours[count][0] = i;
//         neighbours[count][1] = j + 1;
//         count++;
//     }
// }

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
    int height, width;
    
    promptMazeSize(height, width);

    // Allocate memory for height first
    Cell **maze = new Cell*[height];
    // Allocate memory for width
    for (int i = 0; i < height; i++) {
        maze[i] = new Cell[width];
    }

    createMaze(maze, height, width);

    srand(time(0)); // Seed the random number generator
    pickRandomCell(maze, height, width);

    calcFrontier(maze, height, width);

    displayMaze(maze, height, width);
}