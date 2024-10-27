#include <iostream>
#include <queue>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Structure for maze cell
struct Cell {
    int ident = 0;
    bool isvisited = false;
};

// Structure for priority queue
struct Node {
    int height;
    int width;
    int distance; // Manhattan distance to the goal

    // Comparator for priority queue
    bool operator<(const Node& other) const {
        return distance > other.distance;
    }
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

// Function to calculate Manhattan distance
int manhattanDistance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

// Function to check if a move is valid
bool isValidMove(Cell maze[][7], int width, int height, int newHeight, int newWidth) {
    return newHeight >= 0 && newHeight < height && newWidth >= 0 && newWidth < width && maze[newHeight][newWidth].ident != 1 && !maze[newHeight][newWidth].isvisited;
}

// Function to solve the maze using a greedy algorithm
bool solveMaze(Cell maze[][7], int width, int height, int currentHeight, int currentWidth, int endHeight, int endWidth, vector<Node> &path) {
    priority_queue<Node> pq;
    pq.push({currentHeight, currentWidth, manhattanDistance(currentHeight, currentWidth, endHeight, endWidth)});

    while (!pq.empty()) {
        Node current = pq.top();
        pq.pop();

        int curHeight = current.height;
        int curWidth = current.width;

        // If we reached the end
        if (maze[curHeight][curWidth].ident == 3) {
            path.push_back(current);
            return true;
        }

        // Mark the current cell as visited
        maze[curHeight][curWidth].isvisited = true;
        path.push_back(current);

        // Possible moves: down, up, right, left
        int moveHeight[] = {1, -1, 0, 0};
        int moveWidth[] = {0, 0, 1, -1};

        for (int i = 0; i < 4; i++) {
            int newHeight = curHeight + moveHeight[i];
            int newWidth = curWidth + moveWidth[i];

            if (isValidMove(maze, width, height, newHeight, newWidth)) {
                pq.push({newHeight, newWidth, manhattanDistance(newHeight, newWidth, endHeight, endWidth)});
            }
        }
    }

    return false;
}

int main() {
    const int width = 7;
    const int height = 7;
    int currentHeight = 1;
    int currentWidth = 1;
    int endHeight = 5;
    int endWidth = 5;

    vector<Node> path;
    Cell maze[height][width];

    // Initialize the maze
    createMaze(maze, width, height, currentHeight, currentWidth);

    // Display the maze
    displayMaze(maze, width, height);

    // Start the timer
    auto start = high_resolution_clock::now();

    // Solve the maze
    if (solveMaze(maze, width, height, currentHeight, currentWidth, endHeight, endWidth, path)) {
        cout << "\nMaze Solved!" << endl;
        cout << "Path taken:" << endl;
        for (const auto& step : path) {
            if (maze[step.height][step.width].ident != 3 && maze[step.height][step.width].ident != 2) {
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