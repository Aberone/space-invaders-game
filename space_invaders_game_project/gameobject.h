#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

class QString;
class QPainter;

class GameObject
{
public:
    GameObject();

    virtual void act();
    virtual void spawn(int x, int y);
    virtual void die();
    virtual void draw(QPainter* painter);

    virtual int x();
    virtual int width();
    virtual int y();
    virtual int height();
    virtual QString signature();
    virtual bool is_visible();

    virtual ~GameObject();
};

#endif // GAMEOBJECT_H
