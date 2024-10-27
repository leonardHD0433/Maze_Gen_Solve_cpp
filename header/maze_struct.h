#ifndef MAZE_STRUCT_H
#define MAZE_STRUCT_H

// Namespace for structure
namespace maze_struct
{
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

    // Structure to store values for move direction
    struct CellPtr {
        int heightVal=0;
        int widthVal=0;
    };
} 

#endif