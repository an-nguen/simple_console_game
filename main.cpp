#include <iostream>
#include <memory>

#include "core/game_core.h"
#include "models/player.h"


int main() {
    std::shared_ptr<console_game::Player> mainActor(new console_game::Player(5, 5, 5));
    console_game::GameApplication app(mainActor);
    app.gameLoop();

    return 0;
}
