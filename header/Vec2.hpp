#ifndef PHYSOLVER_VEC2_HPP
#define PHYSOLVER_VEC2_HPP
#include <ostream>
class Vec2 {
public:
    double x;
    double y;
    Vec2(double _x = 0, double _y = 0):x(_x), y(_y) {};
    Vec2 operator+(const Vec2& v) const;
    Vec2& operator+=(const Vec2& v);
    Vec2& operator-=(const Vec2& v);
    Vec2 operator-(const Vec2& v) const;
    Vec2 operator-() const;
    Vec2 operator*(const Vec2& v) const;
    Vec2 operator*(const double& a) const;
    Vec2 operator/(const double& a) const;
    static Vec2 zero();
    Vec2 normal() const;
    Vec2 normalize() const;
    double magnitude() const;
    double dot(const Vec2& v) const;
    double cross(const Vec2& v) const;

    friend std::ostream& operator<<(std::ostream& os, const Vec2& vec);
};

inline std::ostream& operator<<(std::ostream& os, const Vec2& vec) {
    os << "(" << vec.x << ", " << vec.y << ")";
    return os;
}

#endif
