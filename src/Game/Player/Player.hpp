#pragma once
#include "Actor.hpp"
#include "CameraController.hpp"
#include "Utils.hpp"

class Cannon;
class Tank;
class Imgui;
class Input;
class InputDispatcher;

class Player : public Actor
{
private:
    std::shared_ptr<Input> m_input;

    void initInputContext();
    void looseFocus();
public:
    Player(InputDispatcher&);
    ~Player();
    void update(float dt);
    void updateGraphic(float dt);

};
