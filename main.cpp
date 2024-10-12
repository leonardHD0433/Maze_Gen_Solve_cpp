/*How Its supposed to work:

1. Init the maze in main
2. The current coordinates are init as the start box
3. An adjacent box is picked if it has not been visited prior and the previous coordinates are added to the history stack.
4. Process continues until end is reached or no path forward is found.
5. If no path forward is found, return to previous box and look for other paths.
6. If the current coordinates return all the way to start, maze has no solution (logic needs to be fixed here as going back to start dosen't necessarily mean no solutions)




*/













#include <iostream>
#include <stack>
using namespace std;

//Structure for maze cell
struct Cell{
    int ident=0;
    bool isvisited=false;
};

//structure for stack history tracking
struct Hist{
    int height;
    int width;
};

//its either OP af or broken af. Pointers in 2D arrays SUCK
int nextTileSelect( Cell* maze,int currentHeight, int currentWidth, int width){
    Cell nextTile;
    int count=0;
    do{
        count++;

        switch(count){
            case 0: nextTile= *((maze+(currentHeight+1) *width)+currentWidth); break;
            case 1: nextTile= *((maze+(currentHeight-1) *width)+currentWidth); break;
            case 2: nextTile= *((maze+(currentHeight) *width)+(currentWidth+1)); break;
            case 3: nextTile= *((maze+(currentHeight) *width)+(currentWidth-1)); break;
            default: break;
        }

    } while ((nextTile.ident==1)||(nextTile.isvisited=true)||(count<4));
    return count;
}


int traverse(Cell* maze,int & currentHeight, int & currentWidth, int width, int dir, stack<Hist>& history){
    if (dir==4){
        //Go a step back, no available tiles to proceed
        //Reached back to start with no more solutions (exit)
        if(history.empty()){
            return 0;
        }

        //move to prev tile
        currentHeight=history.top().height;
        currentWidth=history.top().width;
        history.pop();
        

    }
    else{
        //save tile to stack and go to next tile
        Hist add;
        add.height=currentHeight;
        add.width=currentWidth;
        history.push(add);

        switch(dir){
            case 0: currentHeight++; break;
            case 1: currentHeight--; break;
            case 2: currentWidth++; break;
            default: currentWidth--; break;
        }

        //end if maze end is found
        if((*((maze+(currentHeight) *width)+currentWidth)).ident==3){
            return 1;
        }

        //change new tile to "visited"
        (*((maze+(currentHeight) *width)+currentWidth)).isvisited=true;
        return 2;
    }
}




int main(){


    //building a maze should be in a func if possible
    int width=7;
    int height=7;
    int currentHeight=1;
    int currentWidth=1;
    int end=0;

    stack<Hist> history;
    Cell maze[height][width];
   
   //Set corner values to blocked
    for (int i=0;i<width;i++){
        maze[0][i].ident=1;
        maze[height-1][i].ident=1;
    }

    for (int i=0;i<height;i++){
        maze[i][0].ident=1;
        maze[i][width-1].ident=1;
    }

    //Set corner values to blocked

    
    //Declare Blocked(1), Start (2) and End (3)
    maze[currentHeight][currentWidth].ident= 2;
    maze[5][5].ident=3;
    maze [2][1].ident=1, maze[2][3].ident=1, maze[2][4].ident=1, maze[4][2].ident=1, maze[4][4].ident=1, maze[4][5].ident=1;


    for (int i=0; i<width*2+1;i++){
        cout<<"_";
    }

    cout<<endl;
    for (int i=0;i<height;i++){
        cout<<"|";
        for (int j=0;j<width;j++){
            switch(maze[i][j].ident){
                case 1: cout<<"X|"; break;
                case 2: cout<<"S|"; break;
                case 3: cout<<"E|"; break;
                default: cout<<" |"; break;
            }
        }
        cout<<endl;
    }

     for (int i=0; i<width*2+1;i++){
        cout<<"‾";
    }


    //maze tile movement
    do{
        
        //select next open, unexplored tile direction
        int dir=nextTileSelect((Cell*) maze,currentHeight,currentWidth,width);

        //move to next tile
        end=traverse((Cell*) maze,currentHeight,currentWidth,width,dir,history);
    }while(end==2);

    if (end==0){
        cout<<"Maze has no Solution";
    }
    else{
        //display solution based on stacck content
    }



}
