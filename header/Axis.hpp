#ifndef PHYSOLVER_AXIS_HPP
#define PHYSOLVER_AXIS_HPP

class Axis { // osa reprezentována jako jednickový vektor
public:
    Axis(double _x = 1, double _y = 1);
    double getX() const;
    double getY() const;
private:
    double x;
    double y;
    void normalize();

};

#endif
