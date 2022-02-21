#include "label.h"
#include "engine.h"
#include <QString>
#include <QImage>
#include <QRect>
#include <QPainter>


Label::Label(Engine *host)
{
    _host = host;
    _grid = host->grid();
}

QString Label::signature()
{
    return QString("Label");
}

bool Label::is_visible()
{
    return _is_visible;
}

void Label::spawn(int x, int y)
{
    bool spawnpoints_are_free = true;
    int width = _grid->width() * 5;
    int height = _grid->height() * 5;
    for (int i = y; i < y + _height_px && spawnpoints_are_free; i += 5) {
        for (int j = x; j < x + _width_px && spawnpoints_are_free; j += 5) {
            if (  !((0 <= i && i < height) || (0 <= j && j < width))
            ||    _grid->is_owned(j / 5, i / 5)  ) {
                spawnpoints_are_free = false;
            }
        }
    }
    if (spawnpoints_are_free) {
        for (int i = y; i < y + _height_px && spawnpoints_are_free; i += 5) {
            for (int j = x; j < x + _width_px && spawnpoints_are_free; j += 5) {
                this->_grid->own(j / 5, i / 5, this);
            }
        }
        _x = x;
        _y = y;
        _is_visible = true;
    }
}

void Label::act()
{

}

void Label::die()
{
    for (int i = _y; i < _y + _height_px; i += 5) {
        for (int j = _x; j < _x + _width_px; j += 5) {
            _grid->free(j / 5, i / 5);
        }
    }
    _is_visible = false;
}

int Label::x()
{
    return _x;
}

int Label::width()
{
    return _width_px;
}

int Label::y()
{
    return _y;
}

int Label::height()
{
    return _height_px;
}

int Label::value()
{
    return _value;
}

void Label::set_geometry(int width, int height)
{
    _width_px = width;
    _height_px = height;
}

void Label::set_text(QString text)
{
    _text = text;
}

void Label::set_value(int value)
{
    _value = value;
}

void Label::draw(QPainter *painter)
{
    if (_text.size() > 0 && _is_visible) {
        painter->save();

        QString display_text = _text + ": " + QString::number(_value);

        QFont font = painter->font();
        font.setPointSize(font.pointSize() * 3);
        painter->setFont(font);
        painter->drawText(_x, _y, _width_px, _height_px, 0, display_text);

        painter->restore();
    }
}
