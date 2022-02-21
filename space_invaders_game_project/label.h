#ifndef LABEL_H
#define LABEL_H

#include "gameobject.h"
#include <QString>
class Engine;
class Grid;
class QPainter;


class Label : public GameObject
{
public:
    Label(Engine* host);

    QString signature();

    bool is_visible();

    void spawn(int x, int y);
    void act();
    void die();

    int value();
    void set_geometry(int width, int height);
    void set_text(QString text);
    void set_value(int value);

    int x();
    int width();
    int y();
    int height();

    void draw(QPainter* painter);

private:
    Engine* _host;
    Grid* _grid;

    int _width_px = 1;
    int _height_px = 1;
    bool _is_visible = false;
    int _x;
    int _y;

    QString _text = "";
    int _value = 0;
};

#endif // LABEL_H
