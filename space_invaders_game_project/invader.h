#ifndef INVADER_H
#define INVADER_H

#include "gameobject.h"
#include <QString>
class Engine;
class Grid;
class QPainter;

class Invader : public GameObject
{
public:
    Invader(Engine* host);

    QString signature();

    bool is_visible();

    void spawn(int x, int y);
    void act();
    void move(); // предполагает, что движение уже возможно совершить
    void die();

    bool can_shoot();
    void give_ammo();

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

    bool _can_shoot = false;
    bool _is_visible = false;
    int _x;
    int _y;
    int _dx = 0;
    int _dy = 2;
    QString _speed_mode = "down_to_right";
    int _speed_change_cd = 0;
};

#endif // INVADER_H
