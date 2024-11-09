#ifndef PHYSOLVER_VEC2_HPP
#define PHYSOLVER_VEC2_HPP
class Vec2 {
public:
    double x;
    double y;
    Vec2(double _x = 0, double _y = 0):x(_x), y(_y) {};
    Vec2 operator+(const Vec2& v) const;
    Vec2& operator+=(const Vec2& v);
    Vec2 operator-(const Vec2& v) const;
    Vec2 operator*(const Vec2& v) const;
    Vec2 operator*(const double& a) const;
    Vec2 operator/(const double& a) const;
    Vec2 normal() const;
    double magnitude() const;
};

#endif
