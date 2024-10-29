#ifndef PHYSOLVER_VEC2_HPP
#define PHYSOLVER_VEC2_HPP
class Vec2 {
public:
    double x;
    double y;
    Vec2(double x_ = 0, double y_ = 0);
    Vec2 operator+(const Vec2 v) const;
    Vec2 operator-(const Vec2 v) const;
    Vec2 operator*(const Vec2 v) const;
    double magnitude() const;
};

#endif
