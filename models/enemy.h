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
    class Enemy : public GameLivingObject {
    private:
        int strength = 5;
        int agility = 5;
        int endurance = 5;
        double minHitChance = 0.1;
        double maxHitChance = 1;
    public:
        Enemy() = default;
        explicit Enemy(int strength, int agility, int endurance, int baseDamage)
                : strength(strength), agility(agility), endurance(endurance) {
            this->baseDamage = baseDamage;
            this->healthPoints = getMaxHealth();
        }

        double getMaxHealth() override {
            return this->endurance * 10;
        }

        double calcDmg() override {
            std::random_device r;
            std::default_random_engine e1(r());
            std::uniform_int_distribution<int> uniform_dist(minHitChance, maxHitChance);
            auto hit_chance = uniform_dist(e1);
            return strength * hit_chance + (agility * 0.5) + this->baseDamage;
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

        friend std::ostream & operator<<(std::ostream &os, Enemy e) {
            os << "Enemy{health=" << e.healthPoints << "/" << e.getMaxHealth() << "}";
            return os;
        }

        std::string toString() {
            std::stringstream out;
            out << "Enemy{health=" << healthPoints << "/" << this->getMaxHealth() << "}";
            return out.str();
        }
    };
}

#endif //CONSOLE_GAME_ENEMY_H
