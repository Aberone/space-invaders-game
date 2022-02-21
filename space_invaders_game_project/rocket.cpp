#include "rocket.h"
#include "engine.h"
#include <QString>
#include <QImage>
#include <QRect>
#include <QPainter>


Rocket::Rocket(Engine* host)
{
    _host = host;
    _grid = host->grid();
}

QString Rocket::signature()
{
    return QString("Rocket");
}

bool Rocket::is_visible()
{
    return _is_visible;
}

void Rocket::spawn(int x, int y)
{
    bool spawnpoints_are_free = true;
    int width = _grid->width() * 5;
    int height = _grid->height() * 5;
    for (int i = y; i < y + _HEIGHT_PX && spawnpoints_are_free; i += 5) {
        for (int j = x; j < x + _WIDTH_PX && spawnpoints_are_free; j += 5) {
            if (  !((0 <= i && i < height) || (0 <= j && j < width))
            ||    _grid->is_owned(j / 5, i / 5)  ) {
                spawnpoints_are_free = false;
            }
        }
    }
    if (spawnpoints_are_free) {
        for (int i = y; i < y + _HEIGHT_PX && spawnpoints_are_free; i += 5) {
            for (int j = x; j < x + _WIDTH_PX && spawnpoints_are_free; j += 5) {
                this->_grid->own(j / 5, i / 5, this);
            }
        }
        _x = x;
        _y = y;
        _is_visible = true;
    }
}

void Rocket::act()
{
    // Пытаемся двигаться вверх, есчё обрабатываем коллизии или самоуничтожение от столкновения с потолком
    bool move_space_is_free = true;
    GameObject* owner;
    int newx = _x + _dx, newy = _y + _dy;
    for (int i = newy; i < newy + _HEIGHT_PX && move_space_is_free; i += 5) {
        for (int j = newx; j < newx + _WIDTH_PX && move_space_is_free; j += 5) {
            if (!(_grid->is_free(j / 5, i / 5) || _grid->owner(j / 5, i / 5) == this)) {
                move_space_is_free = false;
                owner = _grid->owner(j / 5, i / 5);
            }
        }
    }
    if (move_space_is_free) {
        this->move();
    }
    else {
        QVector <GameObject*> collision_info;
        collision_info.append(this);
        collision_info.append(owner);
        _host->recieve_message("collision", collision_info);
    }
}

void Rocket::move()
{
    for (int i = _y; i < _y + _HEIGHT_PX; i += 5) {
        for (int j = _x; j < _x + _WIDTH_PX; j += 5) {
            _grid->free(j / 5, i / 5);
            _grid->own((j + _dx) / 5, (i + _dy) / 5, this);
        }
    }
    _x += _dx;
    _y += _dy;
}

void Rocket::die()
{
    for (int i = _y; i < _y + _HEIGHT_PX; i += 5) {
        for (int j = _x; j < _x + _WIDTH_PX; j += 5) {
            _grid->free(j / 5, i / 5);
        }
    }
    _is_visible = false;
}

int Rocket::x()
{
    return _x;
}

int Rocket::width()
{
    return _WIDTH_PX;
}

int Rocket::y()
{
    return _y;
}

int Rocket::height()
{
    return _HEIGHT_PX;
}

void Rocket::draw(QPainter *painter)
{
    if (_is_visible) {
        QImage sprite(QString(":/rocket.png"));
        painter->drawImage(QRect(_x, _y, _WIDTH_PX, _HEIGHT_PX), sprite);
    }
}
