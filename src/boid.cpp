#include "boid.h"
#include "screen.h"
#include <iostream>
#include <iomanip>
#include <GLFW/glfw3.h>
#include <cmath>
#include <vector>
#include <random>

using namespace std;

Boid::Boid(Screen &scr, int ID) : screen(scr), id(ID){
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> dist(-MAX_SPEED/2.0f, MAX_SPEED/2.0f);

    mouseRadius = 500.0f;

    screenWidth = screen.getScreenWidth();
    screenHeight = screen.getScreenHeight();

    position.x = rand() % screen.getScreenWidth();
    position.y = rand() % screen.getScreenHeight();
    
    velocity.x = dist(gen);
    velocity.y = dist(gen);

    acceleration = Vector2();

    size = 15.0;
}

void Boid::draw() const {
    Vector2 dir = velocity.normalized();

    Vector2 perp = {-dir.y, dir.x};

    Vector2 tip = position + dir * size;
    Vector2 left = position - dir * size * 0.5 + perp * size * 0.5;
    Vector2 right = position - dir * size * 0.5 - perp * size * 0.5;

    float speed = velocity.length();
    glColor3f( 1/2 * (speed/MAX_SPEED), MAX_SPEED - (speed/MAX_SPEED), (speed/MAX_SPEED));

    glBegin(GL_TRIANGLES);
        glVertex2f(tip.x, tip.y);
        glVertex2f(left.x, left.y);
        glVertex2f(right.x, right.y);
    glEnd();
}

void Boid::updateAcceleration(const vector<Boid>& boids) {
    Vector2 separation;
    Vector2 alignment;
    Vector2 cohesion;
    Vector2 edgeAcceleration;
    Vector2 mouseForce;

    int neighborCount = 0;
    float neighborRadius = 200.0f;
    float separationRadius = 200.0f;
    float maxMouseForce = 300.0f;

    for(const Boid& other : boids) {
        if (&other ==  this) continue;

        Vector2 diff = other.position - position;
        float distance = diff.length();

        if(distance < neighborRadius) {
            if (distance > 0 && distance < separationRadius) {
                separation -= diff.normalized() * (separationRadius - distance) / separationRadius;
            }

            alignment += other.velocity;

            cohesion += other.position;

            neighborCount++;
        }
    }

    float factor = 0;
    float maxEdgeForce = 0.4f * MAX_SPEED;
    if(position.x < EDGE_MARGIN) {
        factor = (EDGE_MARGIN - position.x) / EDGE_MARGIN;
        edgeAcceleration.x += maxEdgeForce * factor;
    }
    if(position.x > screenWidth - EDGE_MARGIN) {
        factor = (position.x - (screenWidth - EDGE_MARGIN)) / EDGE_MARGIN;
        edgeAcceleration.x -= maxEdgeForce * factor;
    }
    if(position.y < EDGE_MARGIN) {
        factor = (EDGE_MARGIN - position.y) / EDGE_MARGIN;
        edgeAcceleration.y += maxEdgeForce * factor;
    }
    if(position.y > screenHeight - EDGE_MARGIN) {
        factor = (position.y - (screenHeight - EDGE_MARGIN)) / EDGE_MARGIN;
        edgeAcceleration.y -= maxEdgeForce * factor;
    }

    if(edgeAcceleration.length() > maxEdgeForce) {
        edgeAcceleration = edgeAcceleration.normalized() * maxEdgeForce;
    }

    Vector2 mouseDiff = position - mouse;
    float mouseDistance = mouseDiff.length();

    if(mouseDistance < mouseRadius && mouseDistance > 0.0f) {
        float factor = (mouseRadius - mouseDistance) / mouseRadius;
        mouseForce = mouseDiff.normalized() * maxMouseForce * factor;
    }

    if (neighborCount > 0) {
        alignment /= static_cast<float>(neighborCount);
        cohesion /= static_cast<float>(neighborCount);

        cohesion = (cohesion - position);

        acceleration = separation * 7.5f + alignment * 0.2f + cohesion * 0.75f + edgeAcceleration * 2.0f + mouseForce;
    }
}

void Boid::update(float dt) {
    float velocityDamper = 0.15;
    velocity += acceleration * dt;

    float speed = velocity.length();
    if (speed > MAX_SPEED) velocity = velocity.normalized() * MAX_SPEED;
    if (speed < MIN_SPEED && speed > 0.0f) velocity = velocity.normalized() * MIN_SPEED;

    position += velocity * dt;

    // // edge wrapping methods
    // if(position.x > screenWidth){position.x = 0.0;}
    // if(position.x < 0.0){position.x = screenWidth;}
    // if(position.y > screenHeight){position.y = 0.0;}
    // if(position.y < 0.0){position.y = screenHeight;}

    velocity *= (1.0 - velocityDamper * dt);

    acceleration = Vector2();
}

void Boid::updateMouse(Vector2 newMouse) {
    newMouse.y = screenHeight - newMouse.y;
    mouse = newMouse;
}