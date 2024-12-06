#include "../header/Math.hpp"
#include <algorithm>
#include <cstdlib>

bool Math::overlap(const Vec2 v1, const Vec2 v2){
    if (v1.x < v2.y && v2.x < v1.y) {
        return true;
    }
    return false;
}

double Math::getOverlap(const Vec2 v1, const Vec2 v2){
    double overlap_start = std::max(v1.x, v2.x);
    double overlap_end = std::min(v1.y, v2.y);

    return std::max(0.0, overlap_end - overlap_start);
}

bool Math::closeEnough(double& a, double&b){
    return std::abs(a - b) < smallNumber;
}

bool Math::closeEnough(Vec2& a, Vec2&b){
    return closeEnough(a.x, b.x) && closeEnough(a.y, b.y);
}

