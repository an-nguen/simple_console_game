//
// Created by an on 8/18/20.
//

#ifndef CONSOLE_GAME_GAME_LIVING_OBJ_H
#define CONSOLE_GAME_GAME_LIVING_OBJ_H

#include <type_traits>

namespace console_game {

    class GameLivingObject {
    protected:
        double m_health{};
        double m_base_health{};
        double m_max_health{};
    public:
        GameLivingObject() = default;
        virtual ~GameLivingObject() = default;

        virtual void setHealth(double health) {
            this->m_health = health;
        }

        [[nodiscard]] virtual double getMaxHealth() const {
            return this->m_max_health;
        }

        virtual void setMaxHealth(double health) {
            this->m_max_health = health;
        }

        [[nodiscard]] virtual double getBaseHealth() const {
            return this->m_base_health;
        }

        virtual void setBaseHealth(double health) {
            this->m_base_health = health;
        }

        [[nodiscard]] virtual double getHealth() const {
            return this->m_health;
        }
    };

    template<class T>
    concept GameLivingObjectDerived = std::is_base_of<GameLivingObject, T>::value;
}

#endif //CONSOLE_GAME_GAME_LIVING_OBJ_H
