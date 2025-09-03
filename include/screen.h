#ifndef SCREEN_H
#define SCREEN_H

#include <vector>

class Boid;

class Screen
{
    public:
        Screen(int x, int y);

        int width;
        int height;
        std::vector<Boid> boids;

        void addBoid(int id);

        // Getters
        int getScreenWidth();
        int getScreenHeight();
};

#endif // SCREEN_H