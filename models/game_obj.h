//
// Created by an on 8/9/20.
//

#ifndef CONSOLE_GAME_GAME_OBJ_H
#define CONSOLE_GAME_GAME_OBJ_H

#include <string>

namespace console_game {
    class GameObj {
    private:
        std::string obj_name;
    public:
        virtual double calcDmg() = 0;
    };

    template<class D>
    concept GameObjDerived = std::is_base_of<GameObj, D>::value;
}

#endif //CONSOLE_GAME_GAME_OBJ_H
