#include "../header/Axis.hpp"

#include <cmath>

Axis::Axis(double _x, double _y): x(_x), y(_y){
    normalize();
}

double Axis::getX() const{
    return x;
}

double Axis::getY() const{
    return y;
}

void Axis::normalize() {
    const double norm = std::sqrt(x * x + y * y);

    x = x / norm;
    y = y / norm;

}
