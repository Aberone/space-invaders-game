#ifndef SHIP_H
#define SHIP_H

#include "gameobject.h"
#include <QString>
class Engine;
class Grid;
class QPainter;

class Ship : public GameObject
{
public:
    Ship(Engine* host);

    QString signature();

    bool is_visible();

    void spawn(int x, int y);
    void act();
    void move(); // предполагает, что движение уже возможно совершить
    void die();

    int x();
    int width();
    int y();
    int height();

    void draw(QPainter* painter);

private:
    Engine* _host;
    Grid* _grid;

    int _WIDTH_PX = 60;
    int _HEIGHT_PX = 60;
    int _SHOOT_CD_MAX = 10;

    bool _is_visible = false;
    int _x;
    int _y;
    int _dx = 0;
    int _dy = 0;
    int _shoot_cd = 0;
};

#endif // SHIP_H
