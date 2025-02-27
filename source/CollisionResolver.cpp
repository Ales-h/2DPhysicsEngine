#include "../header/CollisionResolver.hpp"

#include <iostream>
#include <limits>
#include <stdexcept>
#include <tuple>
#include <vector>

#include "../header/Application.hpp"
#include "../header/Math.hpp"
#include "../header/rectangleShape.hpp"

CollisionResolver::CollisionResolver() {
    _e = 0.3;
    staticFriction = 0.3;
    dynamicFriction = 0.1;
}

CollisionResolver::~CollisionResolver() {}

void CollisionResolver::renderCollisionPoints(Renderer* renderer) {
    std::vector<Vec2> collisionPoints;
    for (Collision c : cpsToRender) {
        for (int i = 0; i < c.contacts.size(); i++) {
            collisionPoints.push_back(c.contacts[i]);
        }
    }
    renderer->drawCollisionPoints(collisionPoints);
    cpsToRender.clear();
}

Vec2 CollisionResolver::detectCollision(const rectangleShape* rect,
                                        const circleShape* circle) {
    double minOverlap = std::numeric_limits<double>::max();
    Axis mtvAxis;
    Vec2 center = circle->center();

    std::vector<Axis> axes = rect->getAxes();

    for (int i = 0; i < axes.size(); ++i) {
        const Axis axis = axes[i];

        Vec2 proj1 = rect->project(axis);
        Vec2 proj2 = circle->project(axis);
        if (!Math::overlap(proj1, proj2)) {
            return Vec2(0, 0);
        } else {
            double ol = Math::getOverlap(proj1, proj2);
            if (ol < minOverlap) {
                minOverlap = ol;
                mtvAxis = axis;
            }
        }
    }
    return Vec2(mtvAxis.getX() * minOverlap, mtvAxis.getY() * minOverlap);
}

Vec2 CollisionResolver::detectCollision(const circleShape* circle,
                                        const rectangleShape* rect) {
    return detectCollision(rect, circle);
}

Vec2 CollisionResolver::detectCollision(const rectangleShape* r1,
                                        const rectangleShape* r2) {
    std::vector<Axis> axes1 = r1->getAxes();
    std::vector<Axis> axes2 = r2->getAxes();
    double minOverlap = std::numeric_limits<double>::max();
    Axis mtvAxis;

    for (int i = 0; i < axes1.size(); ++i) {
        const Axis axis = axes1[i];

        Vec2 proj1 = r1->project(axis);
        Vec2 proj2 = r2->project(axis);
        if (!Math::overlap(proj1, proj2)) {
            return Vec2(0, 0);
        } else {
            double ol = Math::getOverlap(proj1, proj2);
            if (ol < minOverlap) {
                minOverlap = ol;
                mtvAxis = axis;
            }
        }
    }

    for (int i = 0; i < axes2.size(); ++i) {
        const Axis axis = axes2[i];

        Vec2 proj1 = r1->project(axis);
        Vec2 proj2 = r2->project(axis);
        if (!Math::overlap(proj1, proj2)) {
            return Vec2(0, 0);
        } else {
            double ol = Math::getOverlap(proj1, proj2);
            if (ol < minOverlap) {
                minOverlap = ol;
                mtvAxis = axis;
            }
        }
    }
    return Vec2(mtvAxis.getX() * minOverlap, mtvAxis.getY() * minOverlap);
}

Vec2 CollisionResolver::detectCollision(const circleShape* c1, const circleShape* c2) {
    Vec2 distanceVec = c1->center() - c2->center();
    if (distanceVec.magnitude() > c1->radius + c2->radius) {
        return Vec2(0, 0);
    } else {
        Axis axis = Axis(distanceVec);
        double overlap = (c1->radius + c2->radius) - distanceVec.magnitude();

        return Vec2(axis.getX() * overlap, axis.getY() * overlap);
    }
}

Vec2 CollisionResolver::detectCollision(const Shape* shape1, const Shape* shape2) {
    if (auto circle1 = dynamic_cast<const circleShape*>(shape1)) {
        if (auto circle2 = dynamic_cast<const circleShape*>(shape2)) {
            return detectCollision(circle1, circle2);
        } else if (auto rect2 = dynamic_cast<const rectangleShape*>(shape2)) {
            return detectCollision(circle1, rect2);
        }
    } else if (auto rect1 = dynamic_cast<const rectangleShape*>(shape1)) {
        if (auto rect2 = dynamic_cast<const rectangleShape*>(shape2)) {
            return detectCollision(rect1, rect2);
        } else if (auto circle2 = dynamic_cast<const circleShape*>(shape2)) {
            return detectCollision(rect1, circle2);
        }
    }
    throw std::runtime_error("Function for collision detection not found");
    return Vec2(0, 0);
}

void CollisionResolver::resolveCollision(Collision* collision) {
    Vec2 mtv = collision->mtv;
    Object* objectA = collision->A;
    Object* objectB = collision->B;
    std::vector<Vec2> cps = collision->contacts;
    std::array<Vec2, 2> impulses;
    std::array<double, 2> jArray;
    Rigidbody* rbA = objectA->shape->rigidbody;
    Rigidbody* rbB = objectB->shape->rigidbody;
    const double e = _e;
    Vec2 normal = mtv / mtv.magnitude();

    Vec2 raArray[2];
    Vec2 rbArray[2];

    for (int i = 0; i < cps.size(); ++i) {
        Vec2 ra = cps[i] - rbA->pos;
        Vec2 rb = cps[i] - rbB->pos;

        raArray[i] = ra;
        rbArray[i] = rb;

        Vec2 raNormal = ra.normal();
        Vec2 rbNormal = rb.normal();

        Vec2 AngLinVelocityA = raNormal * rbA->omega;
        Vec2 AngLinVelocityB = rbNormal * rbB->omega;

        Vec2 relativeVelocity = (rbB->v + AngLinVelocityB) - (rbA->v + AngLinVelocityA);

        double vAlongNormal = relativeVelocity.dot(normal);

        // collision with fixed bodies dont work with this check
        //  if(vAlongNormal > 0){
        //      impulses.push_back(Vec2::zero());
        //      continue;
        //  }

        double raNdotProd = raNormal.dot(normal);
        double rbNdotProd = rbNormal.dot(normal);

        double j = -(1 + e) * relativeVelocity.dot(normal);
        double inverseMassSum =  (1 / rbA->m) + (1 / rbB->m);

        j /= inverseMassSum + (raNdotProd * raNdotProd) * objectA->inverseInertia() +
             (rbNdotProd * rbNdotProd) * objectB->inverseInertia();
        j /= (double)cps.size();

        Vec2 impulse = normal * j;
        jArray[i] = j;
        impulses[i] = impulse;
    }

    for (int i = 0; i < cps.size(); ++i) {
        Vec2 impulse = impulses[i];
        Vec2 ra = raArray[i];
        Vec2 rb = rbArray[i];
        rbA->v += -impulse / rbA->m;
        rbA->omega += -ra.cross(impulse) * objectA->inverseInertia();
        rbB->v += impulse / rbB->m;
        rbB->omega += (rb.cross(impulse) * objectB->inverseInertia());
        // if (objectA->color == Color::BLUE){
        //std::cout << "vel before A " << rbA->v << "\n";
        //std::cout << "vel before B " << rbB->v << "\n";
        //std::cout << "IMPULSE:" << impulse << "\n";
        //     std::cout << "InverseInertia" << objectA->inverseInertia() << "\n";
        //     std::cout << "omega change:"
        //               << (rb.cross(impulse) * objectA->inverseInertia()) << "\n";
        //     std::cout << "omega " << rbA->omega * (180. / M_PI) << "\n";
        //     std::cout << "theta " << rbA->theta * (180. / M_PI) << "\n";
        //std::cout << "velA " << rbA->v << "\n";

        //     std::cout << "omega change:"
        //               << (rb.cross(impulse) * objectB->inverseInertia()) << "\n";
        //     std::cout << "omega " << rbB->omega * (180. / M_PI) << "\n";
        //     std::cout << "theta " << rbB->theta * (180. / M_PI) << "\n";
        //std::cout << "vel " << rbB->v << "\n";
        // }
    }

    applyFriction(collision, jArray);
    //    rbA->checkRestingPosition();
    //    rbB->checkRestingPosition();
}

void CollisionResolver::applyFriction(Collision* collision,
                                      std::array<double, 2>& jArray) {
    Vec2 mtv = collision->mtv;
    Object* objectA = collision->A;
    Object* objectB = collision->B;
    std::vector<Vec2> cps = collision->contacts;
    Rigidbody* rbA = objectA->shape->rigidbody;
    Rigidbody* rbB = objectB->shape->rigidbody;
    Vec2 normal = mtv / mtv.magnitude();
    std::array<Vec2, 2> frictionImpulseArray;

    Vec2 raArray[2];
    Vec2 rbArray[2];
    for (int i = 0; i < cps.size(); i++) {
        Vec2 ra = cps[i] - rbA->pos;
        Vec2 rb = cps[i] - rbB->pos;

        raArray[i] = ra;
        rbArray[i] = rb;

        Vec2 raNormal = ra.normal();
        Vec2 rbNormal = rb.normal();

        Vec2 angularLinearVelocityA = raNormal * rbA->omega;
        Vec2 angularLinearVelocityB = rbNormal * rbB->omega;

        Vec2 relativeVelocity =
            (rbB->v + angularLinearVelocityB) - (rbA->v + angularLinearVelocityA);

        Vec2 tangent = relativeVelocity - normal * relativeVelocity.dot(normal);

        Vec2 zero = Vec2::zero();
        if (Math::closeEnough(tangent, zero)) {
            continue;
        } else {
            tangent = tangent.normalize();
        }

        double raNdotProd = raNormal.dot(tangent);
        double rbNdotProd = rbNormal.dot(tangent);

        double jt = relativeVelocity.dot(tangent);
        double inverseMassSum = (1 / rbA->m) + (1 / rbB->m);
        jt = -jt /
             (inverseMassSum + (raNdotProd * raNdotProd) * objectA->inverseInertia() +
              (rbNdotProd * rbNdotProd) * objectB->inverseInertia());
        jt /= (double)cps.size();

        Vec2 frictionImpulse;
        double j = jArray[i];

        if (std::abs(jt) <= j * staticFriction) {
            frictionImpulse = -tangent * jt;
        } else {
            frictionImpulse = tangent * -j * dynamicFriction;
            // std::cout << "dynamic" << '\n';
            // std::cout << dynamicFriction << " * " << tangent << " * " << -j << '\n';
            // std::cout << tangent * -j * dynamicFriction << '\n';
            // std::cout << normal << " * " << j <<  '\n';
            // std::cout << normal * j << '\n';
            // std::cout << "tangent " << tangent << '\n';
            // std::cout << "normal " << normal << '\n';
        }

        frictionImpulseArray[i] = frictionImpulse;
    }

    for (int i = 0; i < cps.size(); i++) {
        Vec2 frictionImpulse = frictionImpulseArray[i];
        Vec2 ra = raArray[i];
        Vec2 rb = rbArray[i];

        rbA->v += -frictionImpulse / rbA->m;
        rbA->omega += -ra.cross(frictionImpulse) * objectA->inverseInertia();
        rbB->v += frictionImpulse / rbB->m;
        rbB->omega += (rb.cross(frictionImpulse) * objectB->inverseInertia());
    }
}

// TODO Refactor
void CollisionResolver::resolveMTV(Collision* collision) {
    Vec2 mtv = collision->mtv;
    Object* objectA = collision->A;
    Object* objectB = collision->B;

    bool isFixed1 = objectA->isFixed();
    bool isFixed2 = objectB->isFixed();

    Vec2 direction = objectB->shape->rigidbody->pos - objectA->shape->rigidbody->pos;

    if ((mtv.dot(direction)) < 0) {
        mtv = -mtv;
    }

    if (isFixed1 && !isFixed2) {
        objectB->translate(mtv);
    } else if (!isFixed1 && isFixed2) {
        objectA->translate(-mtv);
    } else if (!isFixed1 && !isFixed2) {
        double mass1 = objectA->shape->rigidbody->m;
        double mass2 = objectB->shape->rigidbody->m;
        double totalMass = mass1 + mass2;

        // if we do mtv based on the mass it results in different impulse outcome
        Vec2 mtv1 = mtv * (mass2 / totalMass);
        Vec2 mtv2 = mtv * (mass1 / totalMass);

        // objectA->translate(-mtv/2.);
        // objectB->translate(mtv/2.);
        objectA->translate(-mtv1);
        objectB->translate(mtv2);
    }
}

void CollisionResolver::checkCollisions(Application* app) {
    std::vector<Collision> collisions = broadPhaseCheck(app->m_objects);

    for (int i = 0; i < collisions.size(); ++i) {
        Vec2 mtv = detectCollision(collisions[i].A->shape, collisions[i].B->shape);
        if (mtv.x == 0 && mtv.y == 0) {
            collisions.erase(collisions.begin() + i);
            i--;
            continue;
        }
        collisions[i].mtv = mtv;
        resolveMTV(&collisions[i]);
        getCollisionPoints(&collisions[i]);
        if (cpsToRender.size() == 0) {
            for (int j = 0; j < collisions.size(); ++j) {
                cpsToRender.push_back(collisions[j]);
            }
        } else {
            for (int j = 0; j < cpsToRender.size(); ++j) {
                if (cpsToRender[j].A == collisions[i].A &&
                    cpsToRender[j].B == collisions[i].B) {
                    cpsToRender[j] = collisions[i];
                    break;
                }
            }
        }
        resolveCollision(&collisions[i]);
    }
}

std::vector<Collision> CollisionResolver::broadPhaseCheck(std::vector<Object*> objects) {
    std::vector<Collision> collisions;
    std::vector<double> radii;  // plural radius
    for (int i = 0; i < objects.size(); ++i) {
        Shape* shape = objects[i]->shape;
        radii.push_back(shape->getRadius());
    }

    for (int i = 0; i < objects.size(); ++i) {
        Vec2 posA = objects[i]->shape->rigidbody->pos;
        for (int j = i + 1; j < objects.size(); ++j) {
            Vec2 posB = objects[j]->shape->rigidbody->pos;
            if (broadPhaseDetection(posA, posB, radii[i], radii[j])) {
                collisions.push_back(Collision(objects[i], objects[j], -1));
            }
        }
    }

    return collisions;
}

// we stay in squared value to avoid square root overhead
bool CollisionResolver::broadPhaseDetection(const Vec2 posA, const Vec2 posB,
                                            const double radiusA, const double radiusB) {
    Vec2 AtoB = posB - posA;
    double distSq = AtoB.dot(AtoB);

    if (distSq < ((radiusA + radiusB) * (radiusA + radiusB))) {
        return true;
    } else {
        return false;
    }
}

void CollisionResolver::getCollisionPoints(Collision* collision) {
    Shape* shapeA = collision->A->shape;
    Shape* shapeB = collision->B->shape;
    std::tuple<int, Vec2, Vec2> tuple;
    if (auto circle1 = dynamic_cast<const circleShape*>(shapeA)) {
        if (auto circle2 = dynamic_cast<const circleShape*>(shapeB)) {
            tuple = getCollisionPoint(circle1->center(), circle1->radius,
                                      circle2->center(), circle2->radius);

        } else if (auto rect2 = dynamic_cast<const rectangleShape*>(shapeB)) {
            auto v2 = rect2->getVertices();
            std::vector<Vec2> vertices2(v2.begin(), v2.end());
            tuple = getCollisionPoint(circle1->center(), circle1->radius, vertices2);
        }
    } else if (auto rect1 = dynamic_cast<const rectangleShape*>(shapeA)) {
        auto v1 = rect1->getVertices();
        std::vector<Vec2> vertices1(v1.begin(), v1.end());
        if (auto rect2 = dynamic_cast<const rectangleShape*>(shapeB)) {
            auto v2 = rect2->getVertices();
            std::vector<Vec2> vertices2(v2.begin(), v2.end());
            tuple = getCollisionPoint(vertices1, vertices2);
        } else if (auto circle2 = dynamic_cast<const circleShape*>(shapeB)) {
            tuple = getCollisionPoint(circle2->center(), circle2->radius, vertices1);
        }
    }
    int cpCount = std::get<0>(tuple);
    collision->contactCounts = cpCount;
    if (cpCount == 1) {
        collision->contacts.push_back(std::get<1>(tuple));
    }
    if (cpCount == 2) {
        collision->contacts.push_back(std::get<1>(tuple));
        collision->contacts.push_back(std::get<2>(tuple));
    }
}

// Circle to Circle collision points
std::tuple<int, Vec2, Vec2> CollisionResolver::getCollisionPoint(const Vec2& centerA,
                                                                 const double& radiusA,
                                                                 const Vec2& centerB,
                                                                 const double& radiusB) {
    Vec2 ab = centerB - centerA;
    Vec2 dir = ab.normalize();
    Vec2 cp = centerA + dir * radiusA;

    return std::tuple<int, Vec2, Vec2>(1, cp, Vec2::zero());
}

std::tuple<int, Vec2, Vec2> CollisionResolver::getCollisionPoint(
    const Vec2& centerA, const double& radiusA, const std::vector<Vec2>& verticesB) {
    Vec2 cp = Vec2::zero();
    double minDist = std::numeric_limits<double>::max();

    for (int i = 0; i < verticesB.size(); ++i) {
        Vec2 va = verticesB[i];
        Vec2 vb = verticesB[(i + 1) % verticesB.size()];
        auto pair = pointSegmentDistance(centerA, va, vb);
        double sqDist = pair.first;
        Vec2 contact = pair.second;

        if (sqDist < minDist) {
            minDist = sqDist;
            cp = contact;
        }
    }
    return std::tuple<int, Vec2, Vec2>(1, cp, Vec2::zero());
}

// polygon to polygon collision points (only tested on rectangles now)
std::tuple<int, Vec2, Vec2> CollisionResolver::getCollisionPoint(
    const std::vector<Vec2>& verticesA, const std::vector<Vec2>& verticesB) {
    double minDist = std::numeric_limits<double>::max();
    int cpCount = 0;
    Vec2 contact1 = Vec2::zero();
    Vec2 contact2 = Vec2::zero();

    for (int i = 0; i < verticesA.size(); ++i) {
        Vec2 p = verticesA[i];

        for (int j = 0; j < verticesB.size(); ++j) {
            Vec2 b1 = verticesB[j];
            Vec2 b2 = verticesB[(j + 1) % verticesB.size()];

            auto pair = pointSegmentDistance(p, b1, b2);
            double sqDist = pair.first;
            Vec2 cp = pair.second;

            if (Math::closeEnough(sqDist, minDist)) {
                if (!Math::closeEnough(cp, contact1)) {
                    contact2 = cp;
                    cpCount = 2;
                }
            } else if (sqDist < minDist) {
                minDist = sqDist;
                cpCount = 1;
                contact1 = cp;
            }
        }
    }

    for (int i = 0; i < verticesB.size(); ++i) {
        Vec2 p = verticesB[i];

        for (int j = 0; j < verticesA.size(); ++j) {
            Vec2 b1 = verticesA[j];
            Vec2 b2 = verticesA[(j + 1) % verticesA.size()];

            auto pair = pointSegmentDistance(p, b1, b2);
            double sqDist = pair.first;
            Vec2 cp = pair.second;

            if (Math::closeEnough(sqDist, minDist)) {
                if (!Math::closeEnough(cp, contact1)) {
                    contact2 = cp;
                    cpCount = 2;
                }
            } else if (sqDist < minDist) {
                minDist = sqDist;
                cpCount = 1;
                contact1 = cp;
            }
        }
    }

    return std::tuple<int, Vec2, Vec2>(cpCount, contact1, contact2);
}

std::pair<double, Vec2> CollisionResolver::pointSegmentDistance(const Vec2& p,
                                                                const Vec2& a,
                                                                const Vec2& b) {
    Vec2 ab = b - a;
    Vec2 ap = p - a;
    Vec2 cp = Vec2::zero();

    double proj = ab.dot(ap);
    double abLengthSq = ab.dot(ab);
    double d = proj / abLengthSq;

    if (d <= 0) {
        cp = a;
    } else if (d >= 1) {
        cp = b;
    } else {
        cp = a + ab * d;
    }
    Vec2 edge = p - cp;
    return std::pair<double, Vec2>(edge.dot(edge), cp);
    // returning squared distance to skip square root operation
}
