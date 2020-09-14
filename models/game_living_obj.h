//
// Created by an on 8/18/20.
//

#ifndef CONSOLE_GAME_GAME_LIVING_OBJ_H
#define CONSOLE_GAME_GAME_LIVING_OBJ_H

#include <type_traits>

namespace console_game {

    class GameLivingObject {
    protected:
        double healthPoints{};
        double baseDamage{};
    public:
        GameLivingObject() = default;
        virtual ~GameLivingObject() = default;

        virtual void setHealth(double health) {
            this->healthPoints = health;
        }

        [[nodiscard]] virtual double getHealth() const {
            return this->healthPoints;
        }

        [[nodiscard]] virtual double getBaseDamage() const {
            return this->baseDamage;
        }

        virtual void setBaseDamage(double damage) {
            this->baseDamage = damage;
        }

        virtual double calcDmg() = 0;
        virtual double getMaxHealth() = 0;
    };

    template<class T>
    concept GameLivingObjectDerived = std::is_base_of<GameLivingObject, T>::value;
}

#endif //CONSOLE_GAME_GAME_LIVING_OBJ_H
