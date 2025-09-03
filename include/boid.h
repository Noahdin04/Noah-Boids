#ifndef BOID_H
#define BOID_H

#include "vector2.h"
#include <vector>

using namespace std;

class Screen;

class Boid {
public:
    Boid(Screen &scr, int ID);  // constructor

    static constexpr float MAX_SPEED = 475.0f;
    static constexpr float MIN_SPEED = 50.0f;
    static constexpr float EDGE_MARGIN = 100.0f;

    int id;

    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;

    float size;
    int screenWidth;
    int screenHeight;
    Vector2 mouse;
    float mouseRadius;

    void updateAcceleration(const vector<Boid>& boids); 
    void update(float dt);
    void draw() const;
    void updateMouse(Vector2 newMouse);

private:
    Screen &screen;     // reference to the screen

};

#endif // BOID_H