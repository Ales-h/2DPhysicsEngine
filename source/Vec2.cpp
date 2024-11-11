#include "../header/Vec2.hpp"
#include <cmath>

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

Vec2 Vec2::operator-() const {
    return Vec2(-x, -y);
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

Vec2 Vec2::normal() const {
    return Vec2(y, -x);
}

double Vec2::magnitude() const {
    return std::sqrt(x * x + y * y);
}

double Vec2::dot(const Vec2& v){
    return x * v.x + y * v.y;
}

