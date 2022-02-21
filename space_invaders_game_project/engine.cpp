#include "engine.h"
#include "grid.h"
#include "gameobject.h"
#include "invader.h"
#include "ship.h"
#include "rocket.h"
#include "bomb.h"
#include "label.h"
#include <QPainter>

Engine::Engine()
    :Engine(800, 600) {}

Engine::Engine(int gamefield_width, int gamefield_height)
{
    _gamefield_width = gamefield_width;
    _gamefield_height = gamefield_height;
    _grid = new Grid(gamefield_width / 5, gamefield_height / 5);

    _keyboard_state["space"] = false;
    _keyboard_state["left"] = false;
    _keyboard_state["right"] = false;

    this->init_game();
}

Grid* Engine::grid()
{
    return _grid;
}

void Engine::init_game()
{
    std::srand(std::time(nullptr));

    _label_life_count = new Label(this);
    _label_life_count->set_text(" Число жизней");
    _label_life_count->set_geometry(_gamefield_width / 2, 50);
    _label_life_count->set_value(5);
    this->add_object(_label_life_count);
    _label_life_count->spawn(0, 0);

    _label_score_count = new Label(this);
    _label_score_count->set_text("     Счёт");
    _label_score_count->set_geometry(_gamefield_width / 2, 50);
    _label_score_count->set_value(0);
    this->add_object(_label_score_count);
    _label_score_count->spawn(_gamefield_width / 2, 0);

    Label* label_floor = new Label(this);
    label_floor->set_geometry(_gamefield_width, 20);
    this->add_object(label_floor);
    label_floor->spawn(0, _gamefield_height - 20);

    Ship* ship = new Ship(this);
    this->add_object(ship);
    ship->spawn(_gamefield_width / 2 - ship->width() / 2, _gamefield_height - 30 - ship->height());
}

void Engine::init_turn()
{
    int invaders_count = 0;
    for (int i = 0; i < _objects.size(); ++i) {
        if (_objects[i]->signature() == "Invader") {
            ++invaders_count;
        }
    }
    if (invaders_count == 0) {
        if (_invaders_rows == 4) {
            this->victory();
        }
        else {
            this->spawn_invaders(_invaders_per_row, _invaders_rows);
            _invaders_count = _invaders_per_row * _invaders_rows;
            ++_invaders_rows;
        }
    }

    for (int i = 0; i < _objects.size(); ++i) {
        _objects[i]->act();
        for (int j = 0; j < _messages.size(); ++j) {
            this->process_message(_messages[j]);
        }
        _messages.clear();
    }
}

void Engine::draw_game(QPainter* painter)
{
    for (int i = 0; i < _objects.size(); ++i) {
        _objects[i]->draw(painter);
    }
}

void Engine::recieve_message(QString event_name, QVector<GameObject *> affected_entities)
{
    Message new_message;
    new_message.event_name = event_name;
    new_message.affected_entities = affected_entities;
    _messages.append(new_message);
}

void Engine::process_message(Message message)
{
    if (message.event_name == "shoot") {
        GameObject* shooter = message.affected_entities[0];
        if (shooter->signature() == "Ship") {
            Rocket* rocket = new Rocket(this);
            int xpos = shooter->x() + shooter->width() / 2 - rocket->width() / 2;
            int ypos = shooter->y() - rocket->height() - 20;
            rocket->spawn(xpos, ypos);
            if (rocket->is_visible()) {
                this->add_object(rocket);
            }
            else {
                delete rocket;
            }
        }
        else if (shooter->signature() == "Invader") {
            Bomb* bomb = new Bomb(this);
            int xpos = shooter->x() + shooter->width() / 2 - bomb->width() / 2;
            int ypos = shooter->y() + shooter->height() + 20;
            bomb->spawn(xpos, ypos);
            if (bomb->is_visible()) {
                this->add_object(bomb);
            }
            else {
                delete bomb;
            }
        }
    }
    else if (message.event_name == "collision") {
        GameObject* obj1 = message.affected_entities[0];
        GameObject* obj2 = message.affected_entities[1];
        if ((obj1->signature() == "Invader" && obj2->signature() == "Rocket")
        ||  (obj1->signature() == "Rocket" && obj2->signature() == "Invader")) {
            this->delete_object(obj1);
            this->delete_object(obj2);
            --_invaders_count;
            _score += 500;
            _label_score_count->set_value(_score);
        }
        else if (obj1->signature() == "Bomb" && obj2->signature() == "Rocket") {
            this->delete_object(obj2);
        }
        else if (obj1->signature() == "Rocket" && obj2->signature() == "Bomb") {
            this->delete_object(obj1);
        }
        else if (obj1->signature() == "Bomb" && obj2->signature() == "Ship") {
            this->delete_object(obj1);
            this->game_over();
        }
        else if (obj1->signature() == "Ship" && obj2->signature() == "Bomb") {
            this->delete_object(obj2);
            this->game_over();
        }
        else if (obj1->signature() == "Invader" && obj2->signature() == "Ship") {
            this->delete_object(obj1);
            this->game_over();
            --_invaders_count;
            _score += 500;
            _label_score_count->set_value(_score);
        }
        else if (obj1->signature() == "Ship" && obj2->signature() == "Invader") {
            this->delete_object(obj2);
            this->game_over();
            --_invaders_count;
            _score += 500;
            _label_score_count->set_value(_score);
        }

        else if (obj1->signature() == "Bomb" && obj2->signature() == "Label") {
            this->delete_object(obj1);
        }
        else if (obj1->signature() == "Rocket" && obj2->signature() == "Label") {
            this->delete_object(obj1);
        }
        else if (obj1->signature() == "Invader" && obj2->signature() == "Label") {
            this->game_over();
        }

        else if (obj1->signature() == "Bomb" && obj2->signature() == "Bomb") {
            this->delete_object(obj1);
            this->delete_object(obj2);
        }
        else if (obj1->signature() == "Invader" && obj2->signature() == "Bomb") {
            this->delete_object(obj2);
        }
        else if (obj1->signature() == "Bomb" && obj2->signature() == "Invader") {
            this->delete_object(obj1);
        }
    }
}

void Engine::add_object(GameObject *object)
{
    _objects.append(object);
}

void Engine::delete_object(GameObject *object)
{
    object->die();
    for (int i = 0; i < _objects.size(); ++i) {
        if (_objects[i] == object) {
            _objects.removeAt(i);
            break;
        }
    }
    delete object;
}

void Engine::try_update_keyboard_state(QString key_name, bool new_state)
{
    if (key_name == "space") {
        _keyboard_state["space"] = new_state;
    }
    else if (key_name == "left") {
        if (new_state == true) {
            if (_keyboard_state["right"] == false) {
                _keyboard_state["left"] = true;
            }
        }
        else {
            _keyboard_state["left"] = false;
        }
    }
    else if (key_name == "right") {
        if (new_state == true) {
            if (_keyboard_state["left"] == false) {
                _keyboard_state["right"] = true;
            }
        }
        else {
            _keyboard_state["right"] = false;
        }
    }
}

bool Engine::keyboard_state(QString key_name)
{
    return _keyboard_state[key_name];
}

void Engine::spawn_invaders(int invaders_per_row, int rows)
{
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < invaders_per_row; ++j) {
            Invader* invader = new Invader(this);
            if (i == rows - 1) {
                invader->give_ammo();
            }
            invader->spawn(45 + j*(invader->width() + 40), 55 + i*(invader->height() + 30));
            if (invader->is_visible()) {
                this->add_object(invader);
            }
            else {
                delete invader;
            }
        }
    }
}

void Engine::victory()
{
    _is_victory = true;
}

void Engine::game_over()
{
    if (_life_count > 0) {
        --_life_count;
        _label_life_count->set_value(_life_count);
    }
    else {
        _is_game_over = true;
    }
}

bool Engine::is_victory()
{
    return _is_victory;
}

bool Engine::is_game_over()
{
    return _is_game_over;
}

Engine::~Engine()
{
    delete _grid;
}
