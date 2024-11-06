#include "../header/Vec2.hpp"
#include <cmath>
Vec2::Vec2(double x_, double y_):x(x_), y(y_){}

Vec2 Vec2::operator+(const Vec2& v) const {
    return Vec2(x + v.x, y + v.y);
}


Vec2& Vec2::operator+=(const Vec2& v){
    x += v.x;
    y += v.y;
    return *this;
}

Vec2 Vec2::operator-(const Vec2& v) const {
    return Vec2(x - v.x, y - v.y);
}

Vec2 Vec2::operator*(const Vec2& v) const {
    return Vec2(x * v.x, y * v.y);
}

Vec2 Vec2::operator*(const double& a) const {
    return Vec2(x * a, y * a);
}

Vec2 Vec2::operator/(const double& a) const {
    return Vec2(x / a, y / a);
}

double Vec2::magnitude() const {
    return std::sqrt(x * x + y * y);
}

