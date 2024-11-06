#ifndef PHYSOLVER_SHAPE_HPP
#define PHYSOLVER_SHAPE_HPP

class Shape{
public:
    Shape();
    ~Shape();
    virtual void render(Renderer* renderer);
private:
};

#endif
