//
// Created by an on 21.08.2020.
//

#ifndef CONSOLE_GAME_GAME_FUNCTIONS_H
#define CONSOLE_GAME_GAME_FUNCTIONS_H

#include <json/json.h>
#include <fstream>

#include "game_core.h"

namespace console_game {
    void save_game(const char * filepath, GameApplication &app) {
        std::ofstream save_file(filepath);
        save_file << app.getMainActor()->to_json();
    }
}

#endif //CONSOLE_GAME_GAME_FUNCTIONS_H
