#include "gameobject.h"
#include <QString>
#include <QPainter>

GameObject::GameObject()
{

}

QString GameObject::signature()
{
    return QString("");
}

bool GameObject::is_visible()
{
    return false;
}

GameObject::~GameObject()
{

}


void GameObject::act()
{

}

void GameObject::spawn(int x, int y)
{

}

void GameObject::die()
{

}

void GameObject::draw(QPainter* painter)
{

}

int GameObject::x()
{
    return 0;
}

int GameObject::width()
{
    return 0;
}

int GameObject::y()
{
    return 0;
}

int GameObject::height()
{
    return 0;
}
