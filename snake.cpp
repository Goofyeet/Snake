//#include "snake.hpp"
#include <stdlib.h>
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <deque>
#include <vector>
//#include <time.h>

using namespace std;

bool gameOver = false;
const int width = 20;
const int height = 20;
int foodX=7;
int foodY=15;
int tailSize = 0;
deque<int> xCoords;
deque<int> yCoords;
vector<int> emptyX;
vector<int> emptyY;
int headX = 10;
int headY = 10;
int prevX;
int prevY;

enum direction {up, righty, down, lefty, stop};
direction dir = stop;

bool checkConflict(int xPos, int yPos){
    for(auto pos = 1; pos < tailSize; pos++){
        if(xCoords[pos] == xPos){
            if(yCoords[pos] == yPos){
                return true;
            }
        }
    }
    return false;
    }

bool isTailHere(int x, int y){
    for(auto pos = 1; pos <= tailSize; pos++){
        if((xCoords[pos] == x) && (yCoords[pos] == y)){
            return true;
        }
    }
    return false;
}

void hitBorder(){
    if(headX == 1){
      headX = width - 1;  
    }
    else if(headX == width){
        headX = 2;
    }
    if(headY == 1){
        headY = height - 1;
    }
    else if(headY == height){
        headY = 2;
    }
}

void setup(){

}

void draw(){
    system("cls");
    
    for(int y = 1; y <= height; y++){
        cout << endl;
        switch (y)
        {
        case 1:
        case height:
            for(int x = 1; x <= width; x++){
                cout << '#';
            }
            continue;

        //y is in between top and bottom borders
        default:
            for(int x = 1; x <= width; x++){
                if((x == 1) || (x == width)){
                    cout << '#';
                }
                else{
                    if((x == headX) && (y == headY)){
                        cout << 'O';
                    }
                    else if((x == foodX) && (y == foodY)){
                        cout << '@';
                    }
                    else if(isTailHere(x,y)){
                        cout << 'o';
                    }
                    else{
                        cout << ' '; 
                    }
                    
                }
        }
        }
    }
    cout << endl;
    cout << "tail Size: " << tailSize << endl;
}

void input(){
    if(kbhit()){
        char key = getch();
        switch(key){
        case 'w':
            dir = up;
            headY -= 1;
            break;
        case 'a':
            dir = lefty;
            headX -= 1;
            break;
        case 's':
            dir = down;
            headY += 1;
            break;
        case 'd':
            dir = righty;
            headX += 1;
            break;
        default:
            dir = stop;
        }
    }
    else{
        dir = stop;
    }
}

void logic(){
    if(dir != stop){
//check if you hit the border
    hitBorder();
    xCoords.push_front(headX);
    yCoords.push_front(headY);

    //check if you ate food
    if ((headX == foodX) && (headY == foodY)){
        tailSize += 1;
        do{
        foodX = rand() % (width-2) + 2;
        foodY = rand() % (height-2) + 2;
        }
        while(checkConflict(foodX, foodY) || ((foodX == headX) && (foodY == headY)));
    }

    //you just moved...thats it
    else{
        prevX = xCoords.back();
        prevY = yCoords.back();
        xCoords.pop_back();
        yCoords.pop_back();
    }
    
    if(checkConflict(headX, headY)){
        gameOver = true;
    }

    }
}

int main(){

    setup();

    while(!gameOver){
        if(dir != stop){
           draw(); 
        }
        input();
        logic();
        //Sleep(50);
    }

    return 0;
}