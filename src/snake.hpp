#pragma once
#include <deque>
#include <stdlib.h>
#include <chrono>

namespace Snakey
{

    extern bool gameOver;
    const int width = 20;
    const int height = 20;
    extern int tailSize;
    extern std::deque<int> xCoords;
    extern std::deque<int> yCoords;
    extern int foodX;
    extern int foodY;
    extern int headX;
    extern int headY;
    extern int prevY;
    extern int prevX;
    enum direction
    {
        stop = 0,
        up = 1,
        righty = 2,
        down = -1,
        lefty = -2
    };
    extern direction dir;
    extern direction prevDir;
    extern std::chrono::duration<double> timeElapsed;
    double const timeDelay = 0.250;
    extern int count;

    void foodGen();

    bool isTailHere(int x, int y);

    void hitBorder();

    void setup();

    void draw();

    void input();

    void logic();
}
