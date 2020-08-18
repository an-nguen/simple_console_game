//
// Created by an on 8/18/20.
//

#ifndef CONSOLE_GAME_ENEMY_H
#define CONSOLE_GAME_ENEMY_H

#include <random>
#include <memory>

#include "game_obj.h"
#include "game_living_obj.h"

namespace console_game {
    class Enemy : public GameObj, public GameLivingObject {
    private:
        int m_strength = 5;
        int m_agility = 5;
        int m_endurance = 5;
        int m_base_damage = 5;
        double m_min_hit_chance = 0.1;
        double m_max_hit_chance = 1;
    public:
        Enemy() = default;
        explicit Enemy(int mStrength, int mAgility, int mEndurance, int mBaseDamage)
                : m_strength(mStrength), m_agility(mAgility), m_endurance(mEndurance), m_base_damage(mBaseDamage) {
            calcTotalHP();
        }

        void calcTotalHP() {
            this->m_health = this->m_base_health + this->m_endurance * 12;
            this->m_max_health = this->m_health;

        }

        double calcDmg() override {
            std::random_device r;
            std::default_random_engine e1(r());
            std::uniform_int_distribution<int> uniform_dist(m_min_hit_chance, m_max_hit_chance);
            auto hit_chance = uniform_dist(e1);
            return m_strength * (m_agility * 0.5) * hit_chance;
        }

        template<GameLivingObjectDerived T>
        double attack(T target) {
            auto total_dmg = calcDmg();
            target->setHealth(target->getHealth() - total_dmg);
            return total_dmg;
        }
        template<GameLivingObjectDerived T>
        double attack(std::shared_ptr<T> target) {
            auto total_dmg = calcDmg();
            target->setHealth(target->getHealth() - total_dmg);
            return total_dmg;
        }

        ~Enemy() override = default;

        friend std::ostream & operator<<(std::ostream &os, const Enemy &e) {
            os << "Enemy{health=" << e.m_health << "/" << e.m_max_health << "}";
            return os;
        }

        std::string to_string() {
            std::stringstream out;
            out << "Enemy{health=" << m_health << "/" << m_max_health << "}";
            return out.str();
        }
    };
}

#endif //CONSOLE_GAME_ENEMY_H
