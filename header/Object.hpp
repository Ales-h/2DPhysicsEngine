#ifndef PHYSOLVER_OBJECT_HPP
#define PHYSOLVER_OBJECT_HPP

#include "Rigidbody.hpp"
#include "Collider.hpp"

class Object {
public:
    Rigidbody* body;
    Collider* collider;
    // Shape/Collider
};

#endif
