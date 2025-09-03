#ifndef VECTOR2_H
#define VECTOR2_H

#pragma once
#include <iostream>
#include <string>

#include <cmath>

struct Vector2 {
    float x;
    float y;

    Vector2() : x(0), y(0) {};
    Vector2(float x_, float y_) : x(x_), y(y_) {};

    Vector2 operator+(const Vector2& v) const { return {x+v.x, y+v.y};}
    Vector2 operator-(const Vector2& v) const { return {x-v.x, y-v.y};}
    Vector2 operator*(float s) const { return {x*s,y*s};}
    Vector2 operator/(float s) const { return {x/s,y/s};}
    Vector2& operator+=(const Vector2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    Vector2& operator-=(const Vector2& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    Vector2& operator*=(float s) {
        x *= s;
        y *= s;
        return *this;
    }
    Vector2& operator/=(float s) {
        x /= s;
        y /= s;
        return *this;
    }
    float length() const { return sqrt(x*x + y*y);}
    Vector2 normalized() const { 
        float len = length();
        return len > 0 ? *this * (1.0f / len) : Vector2{0,0};
    }
};

inline std::ostream& operator<<(std::ostream& os, const Vector2& p) {
    return os << "(" << p.x << "," << p.y << ")";
}

#endif // VECTOR2_H