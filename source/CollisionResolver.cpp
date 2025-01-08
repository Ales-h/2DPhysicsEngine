#include "../header/CollisionResolver.hpp"

#include <iostream>
#include <limits>
#include <stdexcept>
#include <tuple>
#include <vector>

#include "../header/Application.hpp"
#include "../header/Math.hpp"
#include "../header/rectangleShape.hpp"

CollisionResolver::CollisionResolver() { _e = 0.3; }

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
    std::vector<Vec2> impulses;
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

        //collision with fixed bodies dont work with this check
      //  if(vAlongNormal > 0){
      //      impulses.push_back(Vec2::zero());
      //      continue;
      //  }

        double raNdotProd = raNormal.dot(normal);
        double rbNdotProd = rbNormal.dot(normal);

        double j = -(1 + e) * relativeVelocity.dot(normal);
        double inverseMassSum = (1 / rbA->m) + (1 / rbB->m);

        j /= inverseMassSum + (raNdotProd * raNdotProd) * objectA->inverseInertia() +
             (rbNdotProd * rbNdotProd) * objectB->inverseInertia();
       //j /= static_cast<double>(cps.size());

        Vec2 impulse = normal * j;
        impulses.push_back(impulse);
    }

    for (int i = 0; i < cps.size(); ++i) {
        Vec2 impulse = impulses[i];
        Vec2 ra = raArray[i];
        Vec2 rb = rbArray[i];
        rbA->v += -impulse / rbA->m;
        rbA->omega += -ra.cross(impulse) * objectA->inverseInertia();
        rbB->v += impulse / rbB->m;
        rbB->omega += (rb.cross(impulse) * objectB->inverseInertia());
        if(objectA->color == Object::GREEN || objectB->color == Object::GREEN){
        std::cout << "IMPULSE:" << impulse << "\n";
        std::cout << "InverseInertia" << objectA->inverseInertia() << "\n";
        std::cout << "omega change:" << (rb.cross(impulse) * objectA->inverseInertia())
                  << "\n";
        std::cout << "omega " << rbA->omega * (180. / M_PI) << "\n";
        std::cout << "theta " << rbA->theta * (180. / M_PI) << "\n";
        std::cout << "vel" << rbA->v << "\n";

        std::cout << "vel before" << rbB->v << "\n";
        std::cout << "omega change:" << (rb.cross(impulse) * objectB->inverseInertia())
                  << "\n";
        std::cout << "omega " << rbB->omega * (180. / M_PI) << "\n";
        std::cout << "theta " << rbB->theta * (180. / M_PI) << "\n";
        std::cout << "vel" << rbB->v << "\n";
        }
    }
//    rbA->checkRestingPosition();
//    rbB->checkRestingPosition();
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
    collisions.clear();
    for (int i = 0; i < app->objects.size(); ++i) {
        Object* objectA = app->objects[i];
        for (int j = i + 1; j < app->objects.size(); ++j) {
            Object* objectB = app->objects[j];
            Vec2 mtv = detectCollision(objectA->shape, objectB->shape);
            if (mtv.x == 0 && mtv.y == 0) {
                continue;
            }
            Collision c(objectA, objectB, mtv);
            collisions.push_back(c);
        }
    }

    for (int i = 0; i < collisions.size(); ++i) {
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
