#include "ship.h"
#include "engine.h"
#include <QString>
#include <QImage>
#include <QRect>
#include <QPainter>


Ship::Ship(Engine* host)
{
    _host = host;
    _grid = host->grid();
}

QString Ship::signature()
{
    return QString("Ship");
}

bool Ship::is_visible()
{
    return _is_visible;
}

void Ship::spawn(int x, int y)
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

void Ship::act()
{
    // Если зажата одна из клавиш движения, двигаемся
    if (_host->keyboard_state("left") || _host->keyboard_state("right")) {
        if (_host->keyboard_state("left")) {
            _dx = -10;
        }
        else {
            _dx = 10;
        }
        bool move_space_is_free = true;
        GameObject* owner = nullptr;
        int newx = _x + _dx, newy = _y + _dy;
        int width = _grid->width() * 5;
        for (int i = newy; i < newy + _HEIGHT_PX && move_space_is_free; i += 5) {
            for (int j = newx; j < newx + _WIDTH_PX && move_space_is_free; j += 5) {
                if (j < 10 || j >= width - 10) {
                    move_space_is_free = false;
                }
                else if (!(_grid->is_free(j / 5, i / 5) || _grid->owner(j / 5, i / 5) == this)) {
                    move_space_is_free = false;
                    owner = _grid->owner(j / 5, i / 5);
                }
            }
        }
        if (move_space_is_free) {
            this->move();
        }
        else if (owner) {
            QVector <GameObject*> collision_info;
            collision_info.append(this);
            collision_info.append(owner);
            _host->recieve_message("collision", collision_info);
        }
    }
    // Если откатился кд на стрельбу и зажат пробел, стреляем
    if (_shoot_cd > 0) {
        --_shoot_cd;
    }
    else if (_host->keyboard_state("space")) {
        QVector <GameObject*> shoot_info;
        shoot_info.append(this);
        _host->recieve_message("shoot", shoot_info);
        _shoot_cd = _SHOOT_CD_MAX;
    }
}

void Ship::move()
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

void Ship::die()
{
    for (int i = _y; i < _y + _HEIGHT_PX; i += 5) {
        for (int j = _x; j < _x + _WIDTH_PX; j += 5) {
            _grid->free(j / 5, i / 5);
        }
    }
    _is_visible = false;
}

int Ship::x()
{
    return _x;
}

int Ship::width()
{
    return _WIDTH_PX;
}

int Ship::y()
{
    return _y;
}

int Ship::height()
{
    return _HEIGHT_PX;
}

void Ship::draw(QPainter *painter)
{
    if (_is_visible) {
        QImage sprite(QString(":/spaceship.png"));
        painter->drawImage(QRect(_x, _y, _WIDTH_PX, _HEIGHT_PX), sprite);
    }
}
