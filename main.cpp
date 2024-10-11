#include <iostream>
using namespace std;

struct cell{
    int ident=0;
    bool isvisited=false;
};



int main(){

    int width=5;
    int height=4;
    cell maze[height][width];
   
   //Set all values to 0
    for (int i=0;i<4;i++){
        for (int j=0;j<5;j++){
            maze[i][j].ident=0;
        }
    }
    
    //Declare Blocked(1), Start (2) and End (3)
    maze[0][0].ident= 2;
    maze[3][4].ident=3;
    maze [0][1].ident=1, maze[1][1].ident=1, maze[2][1].ident=1, maze[1][3].ident=1, maze[3][3].ident=1;


    for (int i=0; i<width*2+1;i++){
        cout<<"_";
    }

    cout<<endl;
    for (int i=0;i<4;i++){
        cout<<"|";
        for (int j=0;j<5;j++){
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

}
