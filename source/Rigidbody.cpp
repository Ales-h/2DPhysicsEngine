#include "../header/Rigidbody.hpp"

Rigidbody::Rigidbody(double p_x, double p_y, double v_x, double v_y, double a_x,
                     double a_y, double f_x, double f_y, double _m,
                     double _theta, double _omega, double _epsilon) {
    pos = {p_x, p_y};
    v = {v_x, v_y};
    a = {a_x, a_y};
    f = {f_x, f_y};
    if(_m > 0){
        m = _m;
    } else {
        m = 1; // 1 kg
    }
    theta = _theta;
    omega = _omega;
    epsilon = _epsilon;
}
Rigidbody::~Rigidbody() {
// free when removed from the RigidbodySystem
}

double Rigidbody::getX()const{
    return pos.x;
}

double Rigidbody::getY()const{
    return pos.y;
}
