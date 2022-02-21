#include "invader.h"
#include "engine.h"
#include <QString>
#include <QImage>
#include <QRect>
#include <QPainter>
#include <ctime>
//std::srand(std::time(nullptr));


Invader::Invader(Engine* host)
{
    _host = host;
    _grid = host->grid();
}

QString Invader::signature()
{
    return QString("Invader");
}

bool Invader::is_visible()
{
    return _is_visible;
}

void Invader::spawn(int x, int y)
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

void Invader::act()
{
    // Попытка движения: обновление кулдауна на смену скорости
    if (_speed_change_cd == 0) {
        if (_speed_mode == "right") {
            _speed_mode = "down_to_left";
            _dx = 0;
            _dy = 5;
            _speed_change_cd = 12;
        }
        else if (_speed_mode == "down_to_left") {
            _speed_mode = "left";
            _dx = -5;
            _dy = 0;
            _speed_change_cd = 30;
        }
        else if (_speed_mode == "left") {
            _speed_mode = "down_to_right";
            _dx = 0;
            _dy = 5;
            _speed_change_cd = 12;
        }
        else if (_speed_mode == "down_to_right") {
            _speed_mode = "right";
            _dx = 5;
            _dy = 0;
            _speed_change_cd = 30;
        }
    }
    // Попытка движения: кулдаун на смену скорости не прошёл
    else {
        --_speed_change_cd;
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
    // Может быть, выстрелим, но это не точно
    int shoot_coin_toss = std::rand() % 80;
    if (shoot_coin_toss == 0) {
        QVector <GameObject*> shoot_info;
        shoot_info.append(this);
        _host->recieve_message("shoot", shoot_info);
    }
}

void Invader::move()
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

void Invader::die()
{
    for (int i = _y; i < _y + _HEIGHT_PX; i += 5) {
        for (int j = _x; j < _x + _WIDTH_PX; j += 5) {
            _grid->free(j / 5, i / 5);
        }
    }
    _is_visible = false;
}

bool Invader::can_shoot()
{
    return _can_shoot;
}

void Invader::give_ammo()
{
    _can_shoot = true;
}

int Invader::x()
{
    return _x;
}

int Invader::width()
{
    return _WIDTH_PX;
}

int Invader::y()
{
    return _y;
}

int Invader::height()
{
    return _HEIGHT_PX;
}

void Invader::draw(QPainter* painter)
{
    if (_is_visible) {
        QImage sprite(QString(":/invader.png"));
        painter->drawImage(QRect(_x, _y, _WIDTH_PX, _HEIGHT_PX), sprite);
    }
}
