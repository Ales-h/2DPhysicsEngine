#include "../header/rectangleShape.hpp"
#include <algorithm>
#include <cmath>

rectangleShape::rectangleShape(double _w, double _h){
    width = _w;
    height = _h;
}

rectangleShape::~rectangleShape(){}

void rectangleShape::render(Renderer* renderer){}

Vec2 rectangleShape::center(){
    return rigidbody->pos;
}

std::vector<Vec2> rectangleShape::getVertices() const {
    std::vector<Vec2> vertices(4);
    std::vector<Vec2> local = {
        Vec2{-0.5*width, -0.5*height},
        Vec2{0.5*width, -0.5*height},
        Vec2{-0.5*width, 0.5*height},
        Vec2{0.5*width, 0.5*height},
    };
    for(int i = 0; i < 4; ++i){
        double xlocal = local[i].x;
        double ylocal = local[i].y;
        vertices[i].x = rigidbody->getX() + (std::cos(rigidbody->theta) * xlocal - std::sin(rigidbody->theta * xlocal));
        vertices[i].y = rigidbody->getY() + (std::cos(rigidbody->theta) * ylocal - std::sin(rigidbody->theta * ylocal));
    }
    return vertices;

}

//project the shape onto an axis and returns an interval
Vec2 rectangleShape::project(const Axis& axis){
    std::vector<Vec2> vertices = getVertices();
    std::vector<double> projections(4);
    for(auto v : vertices){
        const double proj = (v.x * axis.getX() + v.y * axis.getY());
        projections.push_back(proj);
    }
    auto interval = std::minmax_element(projections.begin(), projections.end());
    return Vec2{*interval.first, *interval.second};


}