#include "../header/rectangleShape.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <array>
#include <stdexcept>

rectangleShape::rectangleShape(Rigidbody* rb, double _w, double _h, int _id): Shape(rb, 'r'){
    width = _w;
    height = _h;
    id = _id;
    rigidbody = rb;
    type = 'r';
}


rectangleShape::~rectangleShape(){}

void rectangleShape::render(Renderer* renderer, int color){
    auto vertices = getVertices();
    renderer->drawRect(vertices, color);
}

Vec2 rectangleShape::center(){
    return rigidbody->pos;
}

// TODO refactor with array, vector is not needed
std::array<Vec2, 4> rectangleShape::getVertices() const {
    std::array<Vec2, 4> vertices;
    std::array<Vec2, 4> local = {
        Vec2{-0.5*width, -0.5*height}, //bottom left
        Vec2{0.5*width, -0.5*height}, //botom right
        Vec2{0.5*width, 0.5*height}, // top right
        Vec2{-0.5*width, 0.5*height} // top left
    };
    double radians = rigidbody->theta;

    if(rigidbody == nullptr){
    throw std::runtime_error("rigid is null??");
    }

    for(int i = 0; i < 4; ++i){
        double xlocal = local[i].x;
        double ylocal = local[i].y;
        vertices[i].x = rigidbody->getX() + (std::cos(radians) * xlocal - std::sin(radians) * ylocal);
        vertices[i].y = rigidbody->getY() + (std::sin(radians) * xlocal + std::cos(radians) * ylocal);
    }
    return vertices;
}

Vec2 rectangleShape::closestVertex(const Vec2 p) const{
    std::array<Vec2, 4> vertices = getVertices();
    Vec2 closest = vertices[0];
    double minDistance = (vertices[0] - p).magnitude();
    for(int i = 0; i < vertices.size(); ++i){
        double distance = (vertices[i] - p).magnitude();
        if(distance < minDistance){
            minDistance = distance;
            closest = vertices[i];
        }
    }
    return closest;
}

std::vector<Axis> rectangleShape::getAxes() const{
    std::vector<Axis> axes;
    std::array<Vec2, 4> vertices = getVertices();

    for(int i = 0; i < 4; ++i){
        Vec2 p1 = vertices[i];
        Vec2 p2 = vertices[(i+1)%4];

        Vec2 edge = p2 - p1;
        axes.push_back(Axis(edge.normal()));
    }
    return axes;

}
//project the shape onto an axis and returns an interval
Vec2 rectangleShape::project(const Axis& axis) const{
    std::array<Vec2, 4> vertices = getVertices();
    std::vector<double> projections;
    for(auto v : vertices){
        const double proj = (v.x * axis.getX() + v.y * axis.getY());
        projections.push_back(proj);
    }
    auto interval = std::minmax_element(projections.begin(), projections.end());
    return Vec2{*interval.first, *interval.second};
}

double rectangleShape::momentOfInertia() {
    double w = width;
    double h = height;
    return 1./12. * rigidbody->m * (w*w + h*h);
}

