#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

struct Cell {
    int ident = 1; // 1 is blocked by default
    bool isvisited = false;
};

struct Position {
    int x, y;
};

// Global variables
int width, height;
Cell** grid;
Position* walls;
int wallCount;

// Function declarations
void initializeMaze(int w, int h);
void cleanupMaze();
void generateMaze();
void displayMaze();
Position* getFrontier(int x, int y, int& count);
Position* getNeighbours(int x, int y, int& count);
void connectCells(int x1, int y1, int x2, int y2);

int main(int argc, char* argv[]) {
    width = 20;
    height = 20;

    if (argc == 3) {
        width = atoi(argv[1]);
        height = atoi(argv[2]);
    } else if (argc == 2) {
        width = atoi(argv[1]);
        height = width;
    }

    initializeMaze(width, height);
    generateMaze();
    displayMaze();
    cleanupMaze();

    return 0;
}

void initializeMaze(int w, int h) {
    width = w;
    height = h;
    wallCount = 0;

    grid = new Cell*[width];
    for (int i = 0; i < width; i++) {
        grid[i] = new Cell[height];
    }
    walls = new Position[width * height];
}

void cleanupMaze() {
    for (int i = 0; i < width; i++) {
        delete[] grid[i];
    }
    delete[] grid;
    delete[] walls;
}

Position* getFrontier(int x, int y, int& count) {
    static Position frontier[4];
    count = 0;
    if (x > 1 && !grid[x - 2][y].isvisited) frontier[count++] = {x - 2, y};
    if (x + 2 < width && !grid[x + 2][y].isvisited) frontier[count++] = {x + 2, y};
    if (y > 1 && !grid[x][y - 2].isvisited) frontier[count++] = {x, y - 2};
    if (y + 2 < height && !grid[x][y + 2].isvisited) frontier[count++] = {x, y + 2};
    return frontier;
}

Position* getNeighbours(int x, int y, int& count) {
    static Position neighbours[4];
    count = 0;
    if (x > 1 && grid[x - 2][y].isvisited) neighbours[count++] = {x - 2, y};
    if (x + 2 < width && grid[x + 2][y].isvisited) neighbours[count++] = {x + 2, y};
    if (y > 1 && grid[x][y - 2].isvisited) neighbours[count++] = {x, y - 2};
    if (y + 2 < height && grid[x][y + 2].isvisited) neighbours[count++] = {x, y + 2};
    return neighbours;
}

void connectCells(int x1, int y1, int x2, int y2) {
    int x = (x1 + x2) / 2;
    int y = (y1 + y2) / 2;
    grid[x1][y1].isvisited = true;
    grid[x][y].isvisited = true;
    grid[x2][y2].isvisited = true;
    grid[x1][y1].ident = 4;
    grid[x][y].ident = 4;
    grid[x2][y2].ident = 4;
}

void generateMaze() {
    srand(time(0));
    int x = rand() % width;
    int y = rand() % height;
    grid[x][y].isvisited = true;
    grid[x][y].ident = 4;
    int frontierCount;
    Position* frontier = getFrontier(x, y, frontierCount);
    for (int i = 0; i < frontierCount; i++) {
        walls[wallCount++] = frontier[i];
    }

    while (wallCount > 0) {
        int randomIndex = rand() % wallCount;
        Position wall = walls[randomIndex];
        walls[randomIndex] = walls[--wallCount];

        int neighbourCount;
        Position* neighbours = getNeighbours(wall.x, wall.y, neighbourCount);
        if (neighbourCount > 0) {
            int randomNeighbourIndex = rand() % neighbourCount;
            Position neighbour = neighbours[randomNeighbourIndex];
            connectCells(wall.x, wall.y, neighbour.x, neighbour.y);

            Position* newFrontier = getFrontier(wall.x, wall.y, frontierCount);
            for (int i = 0; i < frontierCount; i++) {
                walls[wallCount++] = newFrontier[i];
            }
        }
    }
}

void displayMaze() {
    for (int i = 0; i < width * 2 + 1; i++) {
        cout << "_";
    }
    cout << endl;
    for (int y = 0; y < height; y++) {
        cout << "|";
        for (int x = 0; x < width; x++) {
            if (grid[x][y].ident == 1) {
                cout << "X|"; // Blocked
            } else if (grid[x][y].ident == 2) {
                cout << "S|"; // Start
            } else if (grid[x][y].ident == 3) {
                cout << "E|"; // End
            } else if (grid[x][y].ident == 4) {
                cout << " |"; // Path
            } else {
                cout << " |"; // Empty
            }
        }
        cout << endl;
    }
    for (int i = 0; i < width * 2 + 1; i++) {
        cout << "‾";
    }
    cout << endl;
}