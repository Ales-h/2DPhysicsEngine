#include "../header/ballsScene.hpp"

ballsScene::ballsScene(Application* _app) { app = _app; }

void ballsScene::init() {
    Rigidbody* rb1 = new Rigidbody(4, -5);
    rectangleShape* r1 = new rectangleShape(rb1, 5, 1);
    Object* ob1 = new Object();
    ob1->init(app, r1, Object::FIXED, Object::BLUE);

    Rigidbody* rb2 = new Rigidbody(1, -3);
    rectangleShape* r2 = new rectangleShape(rb2, 1, 5);
    Object* ob2 = new Object();
    ob2->init(app, r2, Object::FIXED, Object::BLUE);

    Rigidbody* rb3 = new Rigidbody(7, -3);
    rectangleShape* r3 = new rectangleShape(rb3, 1, 5);
    Object* ob3 = new Object();
    ob3->init(app, r3, Object::FIXED, Object::BLUE);

    Rigidbody* rb4 = new Rigidbody(4, -1);
    rectangleShape* r4 = new rectangleShape(rb4, 5, 1);
    Object* ob4 = new Object();
    ob4->init(app, r4, Object::FIXED, Object::BLUE);

    Rigidbody* rb5 = new Rigidbody(3, -4);
    rectangleShape* r5 = new rectangleShape(rb5, 0.5, 0.2);
    Object* ob5 = new Object();
    ob5->init(app, r5, Object::DYNAMIC, Object::PURPLE);

    Rigidbody* rb6 = new Rigidbody(4, -4);
    circleShape* s6 = new circleShape(rb6, 0.2);
    Object* ob6 = new Object();
    ob6->init(app, s6, Object::DYNAMIC, Object::YELLOW);

    Rigidbody* rb7 = new Rigidbody(6, -2.3);
    circleShape* s7 = new circleShape(rb7, 0.5);
    Object* ob7 = new Object();
    ob7->init(app, s7, Object::DYNAMIC, Object::YELLOW);

    Rigidbody* rb8 = new Rigidbody(2, -6);
    circleShape* s8 = new circleShape(rb8, 0.1);
    Object* ob8 = new Object();
    ob8->init(app, s8, Object::DYNAMIC, Object::BLUE);

    Rigidbody* rb9 = new Rigidbody(4.3, -2.3);
    circleShape* s9 = new circleShape(rb9, 0.5);
    Object* ob9 = new Object();
    ob9->init(app, s9, Object::DYNAMIC, Object::YELLOW);

    Rigidbody* rb0 = new Rigidbody(5.3, -3.3);
    circleShape* s0 = new circleShape(rb0, 0.3);
    Object* ob0 = new Object();
    ob0->init(app, s0, Object::DYNAMIC, Object::PURPLE);

    Rigidbody* rb11 = new Rigidbody(6, -4);
    rectangleShape* r11 = new rectangleShape(rb11, 0.5, 0.3);
    Object* ob11 = new Object();
    ob11->init(app, r11, Object::DYNAMIC, Object::BLUE);

    rb5->m = 20;
    rb6->m = 10;
    rb7->m = 30;
    rb8->m = 5;
    rb9->m = 50;
    rb0->m = 29;
    rb11->m = 22;

    rb5->v += Vec2(1, 0.3);
    rb6->v += Vec2(0.5, 1);
    rb7->v += Vec2(-1.5, 0.2);
    rb8->v += Vec2(0.8, -1.3);
    rb9->v += Vec2(-2, -1.2);
    rb0->v += Vec2(-0.5, 0.5);
    rb11->v += Vec2(-0.5, 0.5);
}

void ballsScene::clean() {}
