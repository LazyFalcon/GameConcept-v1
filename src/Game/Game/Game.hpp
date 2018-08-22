#pragma once
#include "GameState.hpp"

class Imgui;
class Input;
class InputDispatcher;
class GameUI;
class Settings;

class Game : public GameState
{
private:
    Imgui& m_ui;
    std::shared_ptr<Input> m_input;
    std::unique_ptr<GameUI> m_view;
public:
    Game(Imgui&, InputDispatcher&, Settings&);
    ~Game();
    void update(float dt);
    void updateWithHighPrecision(float dt);
    void renderProcedure(GraphicEngine&);
};
