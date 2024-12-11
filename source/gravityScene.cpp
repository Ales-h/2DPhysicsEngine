#include "../header/gravityScene.hpp"
#include "../header/GravityGenerator.hpp"

gravityScene::gravityScene(Application* _app) {
    app = _app;
}

void gravityScene::init(){
    Rigidbody* rb1 = new Rigidbody(4, -6);
    rectangleShape* r1 = new rectangleShape(7, 1, 13);
    Object* ob1 = new Object();
    ob1->init(app, r1, rb1, Object::FIXED, Object::BLUE);

    Rigidbody* rb2 = new Rigidbody(5, 4);
    rectangleShape* r2 = new rectangleShape(1, 1, 2);
    Object* ob2 = new Object();
    ob2->init(app, r2, rb2, Object::DYNAMIC, Object::PURPLE);

    Rigidbody* rb3 = new Rigidbody(2.5, -2);
    rectangleShape* r3 = new rectangleShape(0.5, 0.2, 5);
    Object* ob3 = new Object();
    ob3->init(app, r3, rb3, Object::DYNAMIC, Object::YELLOW);

    Rigidbody* rb4 = new Rigidbody(7, -1);
    circleShape* s4 = new circleShape(0.5);
    Object* ob4 = new Object();
    ob4->init(app, s4, rb4, Object::DYNAMIC, Object::YELLOW);


    rb1->m = 200;
    rb2->m = 40;
    rb3->m = 30;
    rb4->m = 40;


    r1->rigidbody = rb1;
    r2->rigidbody = rb2;
    r3->rigidbody = rb3;
    s4->rigidbody = rb4;
    //rb2->v.y = -10;
    rb2->theta = 2.1;
    rb3->theta = 1;

    GravityGenerator* gGenerator = new GravityGenerator();
    app->rbSystem->addForceGenerator(gGenerator);
}

void gravityScene::clean(){}
