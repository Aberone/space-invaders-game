#include "grid.h"
#include "gameobject.h"

Grid::Grid()
    :Grid(800, 600){}

Grid::Grid(int width, int height)
{
    this->_width = width;
    this->_height = height;
    this->_data = QVector <QVector <GameObject*> >(height, QVector <GameObject*>(width, nullptr));
}

bool Grid::is_free(int x, int y)
{
    return !(bool)(this->_data[y][x]);
}

bool Grid::is_owned(int x, int y)
{
    return (bool)(this->_data[y][x]);
}

GameObject* Grid::owner(int x, int y)
{
    return this->_data[y][x];
}

void Grid::own(int x, int y, GameObject *object)
{
    this->_data[y][x] = object;
}

void Grid::free(int x, int y)
{
    this->_data[y][x] = nullptr;
}

int Grid::width()
{
    return this->_width;
}

int Grid::height()
{
    return this->_height;
}
