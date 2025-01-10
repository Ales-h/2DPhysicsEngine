#ifndef PHYSOLVER_MATH_HPP
#define PHYSOLVER_MATH_HPP

#include "Vec2.hpp"

// TODO make this a namespace
class Math{
public:
    static bool overlap(const Vec2 v1, const Vec2 v2);
    static double getOverlap(const Vec2 v1, const Vec2 v2);
    static bool closeEnough(double& a, double& b);
    static bool closeEnough(Vec2& a, Vec2& b);
private:
    constexpr static double smallNumber = 0.005;
};

#endif // !PHYSOLVER_MATH_HPP
