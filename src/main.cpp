// imported header files
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <GLFW/glfw3.h>

// manually created header files
#include "vector2.h"
#include "screen.h"
#include "boid.h"

// This is just so I don't have to use std::(insert stdio.h function here) all the time. Even if std::(function) is safer and the "correct" way to do things. This is a personal project, I'm not too worried about it.
using namespace std;

const double FIXED_DT = 1.0 / 60.0;

void drawCircleOutline(Screen screen, float mouseX, float mouseY) {
    mouseY = screen.getScreenHeight() - mouseY;
    glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_LINE_LOOP);
        int segments = 100;
        for(int i = 0; i < segments; i++) {
            float theta = 2.0f * 3.1415926f * float(i) / float(segments);
            float x = screen.boids[0].mouseRadius * cosf(theta);
            float y = screen.boids[0].mouseRadius * sinf(theta);
            glVertex2f(mouseX + x, mouseY + y);
        }
        glEnd;
}

int main() {
    // Initialize random seed using current time
    srand(static_cast<unsigned int>(time(0)));

    // initializes GLFW and returns -1 if unsuccessful
    if (!glfwInit()) return -1;

    // Get the users primary monitor and its pixel dimensions
    GLFWmonitor* primary = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primary);

    int screenWidth = mode->width;
    int screenHeight = mode->height;

    // Initialize an instance of the Screen class with the users monitor resolution
    Screen screen = Screen(screenWidth, screenHeight);

    // put boids on the screen. Eventually this will not be hard coded
    for(int i = 0; i < 150; i++) {
        screen.addBoid(i);
    }

    // Create the OpenGL Window
    GLFWwindow* window = glfwCreateWindow(screenWidth,screenHeight, "Boids", nullptr, nullptr);
    glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_FALSE);
    glfwSetWindowPos(window, 0, 0);

    if (!window) { glfwTerminate(); return -1;}

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, 0, screenHeight, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    double previousTime = glfwGetTime();
    double accumulator = 0.0;
    
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        double currentTime = glfwGetTime();
        double frameTime = currentTime - previousTime;
        previousTime = currentTime;
        accumulator += frameTime;
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        Vector2 mousePos{ static_cast<float>(mouseX), static_cast<float>(mouseY)};

        while(accumulator >= FIXED_DT) {

            for (Boid& b : screen.boids) {
                b.updateMouse(mousePos);
                b.updateAcceleration(screen.boids);
            }

            for (Boid& b : screen.boids) {
                b.update(FIXED_DT);
            }

            accumulator -= FIXED_DT;
        }

        glfwPollEvents();

        // Time in seconds since GLFW started
        float time = static_cast<float>(glfwGetTime());

        // Set background color
        glClearColor(0.1,0.1,0.1,1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        for (Boid& b : screen.boids) {
            b.draw();
        }

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}