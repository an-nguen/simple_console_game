#include "core/game_core.h"
#include "models/player.h"


int main() {
    console_game::Player mainActor(5, 5, 5);
    console_game::GameApplication app(mainActor);
    app.gameLoop();

    return 0;
}
