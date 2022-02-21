#ifndef GRID_H
#define GRID_H

#include <QVector>
class GameObject;

class Grid
{
public:
    Grid();
    Grid(int width, int height);

    bool is_free(int x, int y);
    bool is_owned(int x, int y);
    GameObject* owner(int x, int y);
    void own(int x, int y, GameObject* object);
    void free(int x, int y);

    int width();
    int height();

private:
    int _width;
    int _height;
    QVector <QVector <GameObject*> > _data;
};

#endif // GRID_H
