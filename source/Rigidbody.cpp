#include "../header/Rigidbody.hpp"

#include <string>

const double LINEAR_RESTING_THRESHOLD = 0.001;
const double ANGULAR_RESTING_THRESHOLD = 0.005;

Rigidbody::Rigidbody(double p_x, double p_y, double v_x, double v_y, double a_x,
                     double a_y, double f_x, double f_y, double _m, double _theta,
                     double _omega, double _epsilon) {
    pos = {p_x, p_y};
    v = {v_x, v_y};
    a = {a_x, a_y};
    f = {f_x, f_y};
    if (_m > 0) {
        m = _m;
    } else {
        m = 1;  // 1 kg
    }
    theta = _theta;
    omega = _omega;
    epsilon = _epsilon;
}

Rigidbody::Rigidbody(const Rigidbody& rb)
    : pos(rb.pos),
      v(rb.v),
      a(rb.a),
      f(rb.f),
      m(rb.m),
      theta(rb.theta),
      omega(rb.omega),
      epsilon(rb.epsilon) {}

Rigidbody::~Rigidbody() {
    // free when the Object is destroyed
}

double Rigidbody::getX() const { return pos.x; }

double Rigidbody::getY() const { return pos.y; }

// if the bodies velocity/omega/theta is small we make it a zero because of simulations
// numerical errors + small impulses from the collision response
void Rigidbody::checkRestingPosition() {
    if (v.magnitude() < LINEAR_RESTING_THRESHOLD) {
        v = 0;
    }
    if (abs(omega) < ANGULAR_RESTING_THRESHOLD) {
        omega = 0.0;
    }
}

void Rigidbody::renderVelocityVector(Renderer* renderer) {
    renderer->drawArrow(pos, v, v.magnitude() / 2);
    // TODO show velocity magnitude
    // std::string t = std::to_string(v.magnitude());
    // renderer->renderText(pos, t);
}
