#include "core.hpp"
#include "App.hpp"
#include "Game.hpp"
#include "GameEvents.hpp"
#include "Logging.hpp"
#include "Window.hpp"

bool StartGame::handle(App &app){
    info("[ Start Game ]");

    auto game = std::make_shared<Game>(*app.imgui, *app.inputDispatcher, *app.settings);

    app.window->show();
    app.showMouse();
    app.setGameState(game);
    return true;
}

bool ExitGame::handle(App &app){
    info("[ Bye! ]");
    app.exit();
    return true;
}
