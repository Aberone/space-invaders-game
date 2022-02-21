#ifndef BOMB_H
#define BOMB_H

#include "gameobject.h"
#include <QString>
class Engine;
class Grid;
class QPainter;


class Bomb : public GameObject
{
public:
    Bomb(Engine* host);

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

    int _WIDTH_PX = 20;
    int _HEIGHT_PX = 30;

    bool _is_visible = false;
    int _x;
    int _y;
    int _dx = 0;
    int _dy = 10;
};

#endif // BOMB_H
