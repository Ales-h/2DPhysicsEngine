#include "../headers/Vec2.hpp"
#include <cmath>
Vec2::Vec2(double x_, double y_):x(x_), y(y_){}

Vec2 Vec2::operator+(const Vec2 v) const {
    return Vec2(x + v.x, y + v.y);
}

Vec2 Vec2::operator-(const Vec2 v) const {
    return Vec2(x - v.x, y - v.y);
}

Vec2 Vec2::operator*(const Vec2 v) const {
    return Vec2(x * v.x, y * v.y);
}

double Vec2::magnitude() const {
    return std::sqrt(x * x + y * y);
}

