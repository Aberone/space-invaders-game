#ifndef ENGINE_H
#define ENGINE_H

#include <QVector>
#include <QString>
#include <QMap>
#include "grid.h"
class GameObject;
class QPainter;
class Label;


class Engine
{
public:
    struct Message
    {
        QString event_name; // "collision", "shoot"
        QVector <GameObject*> affected_entities;
    };

    Engine();
    Engine(int gamefield_width, int gamefield_height);

    Grid* grid();

    void init_game();
    void init_turn();
    void draw_game(QPainter* painter);
    void recieve_message(QString event_name, QVector <GameObject*> affected_entities);
    void process_message(Message message);

    void add_object(GameObject* object);
    void delete_object(GameObject* object);

    void try_update_keyboard_state(QString key_name, bool new_state); // "left", "right", "space"
    bool keyboard_state(QString key_name);

    void spawn_invaders(int invaders_per_row, int rows);
    void victory();
    void game_over();
    bool is_victory();
    bool is_game_over();

    ~Engine();

private:
    int _gamefield_width;
    int _gamefield_height;
    Grid* _grid;
    QVector <GameObject*> _objects;
    QVector <Message> _messages;
    QMap <QString, bool> _keyboard_state;

    int _life_count = 5;
    int _score = 0;
    int _invaders_per_row = 6;
    int _invaders_count = 0;
    int _invaders_rows = 1;

    Label* _label_life_count;
    Label* _label_score_count;
    bool _is_game_over = false;
    bool _is_victory = false;
};

#endif // ENGINE_H
