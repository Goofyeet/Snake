#include "snake.hpp"
#include <stdlib.h>
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <deque>

namespace Snakey{

bool gameOver;
int tailSize;
std::deque<int> xCoords;
std::deque<int> yCoords;
int foodX;
int foodY;
int headX;
int headY;
int prevY;
int prevX;
direction dir;

//checks if the passed position is occupied by tail
//only checks against tail not head
bool isTailHere(int x, int y){
    for(auto pos = 1; pos <= tailSize; pos++){
        if((xCoords[pos] == x) && (yCoords[pos] == y)){
            return true;
        }
    }
    return false;
}

void foodGen(){
        do{
        foodX = rand() % (width-2) + 2;
        foodY = rand() % (height-2) + 2;
        }
        while(isTailHere(foodX, foodY) || ((foodX == headX) && (foodY == headY)));
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
    gameOver = false;
    xCoords.clear();
    yCoords.clear();
    tailSize = 0;
    headX = 10;
    headY = 10;
    dir = stop;
    xCoords.push_back(headX);
    yCoords.push_back(headY);

    srand((unsigned) time(NULL));
    foodGen();
}

void draw(){
    system("cls");
    
    for(int y = 1; y <= height; y++){
        std::cout << std::endl;
        switch (y)
        {
        case 1:
        case height:
            for(int x = 1; x <= width; x++){
                std::cout << '#';
            }
            continue;

        //y is in between top and bottom borders
        default:
            for(int x = 1; x <= width; x++){
                if((x == 1) || (x == width)){
                    std::cout << '#';
                }
                else{
                    if((x == headX) && (y == headY)){
                        std::cout << 'O';
                    }
                    else if((x == foodX) && (y == foodY)){
                        std::cout << '@';
                    }
                    else if(isTailHere(x,y)){
                        std::cout << 'o';
                    }
                    else{
                        std::cout << ' '; 
                    }
                    
                }
        }
        }
    }
    std::cout << std::endl;
    std::cout << "tail Size: " << tailSize << std::endl;
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
        foodGen();
    }

    //you just moved...thats it
    else{
        prevX = xCoords.back();
        prevY = yCoords.back();
        xCoords.pop_back();
        yCoords.pop_back();
    }
    
    if(isTailHere(headX, headY)){
        gameOver = true;
    }

    }
}
}
int main(){
    using namespace Snakey;

    while(true){
    setup();

    while(!gameOver){
        if(dir != stop){
           draw(); 
        }
        input();
        logic();
        //Sleep(50);
    }
    }
    return 0;
}
