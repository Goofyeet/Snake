#pragma once
#include <deque>
#include <stdlib.h>

namespace Snakey {

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
enum direction {up, righty, down, lefty, stop};
extern direction dir;

bool isTailHere(int x, int y);

void hitBorder();

void setup();

void draw();

void input();

void logic();
}
