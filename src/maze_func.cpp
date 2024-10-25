#include "../header/maze_func.h"

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
