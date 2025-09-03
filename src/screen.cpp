#include "screen.h"
#include "boid.h"
#include <iostream>
using namespace std;

Screen::Screen(int x, int y) : width(x), height(y) {
    cout << "screen initialized with Width: " << width << ", Height " << height << endl;
}

void Screen::addBoid(int id){
    boids.emplace_back(*this, id);
}

int Screen::getScreenWidth() {
    return width;
}

int Screen::getScreenHeight() {
    return height;
}