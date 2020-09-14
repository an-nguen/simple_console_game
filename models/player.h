//
// Created by an on 8/9/20.
//

#ifndef CONSOLE_GAME_PLAYER_H
#define CONSOLE_GAME_PLAYER_H

#include <random>
#include <sstream>
#include <json/value.h>

#include "game_obj.h"
#include "item.h"
#include "game_living_obj.h"

namespace console_game {
    class Player : public GameLivingObject {
    private:
        double strength = 10;
        double agility = 10;
        double endurance = 10;
        double minHitChance = 0;
        double maxHitChance = 1;
        long long experience = 0;
        long long money = 0;

        std::vector<Item> m_items{};
    public:
        Player() = default;
        explicit Player(double strength = 10,
                        double agility = 10,
                        double endurance = 10,
                        int baseDamage = 10)
                : strength(strength),
                  agility(agility),
                  endurance(endurance) {
            this->baseDamage = baseDamage;
            this->healthPoints = this->getMaxHealth();
        }

        double getMaxHealth() override {
            return this->endurance * 10;
        }

        double calcDmg() override {
            std::random_device r;
            std::default_random_engine e1(r());
            std::uniform_int_distribution<int> uniform_dist(minHitChance, maxHitChance);
            auto hit_chance = uniform_dist(e1);
            return strength * hit_chance + (agility * 0.5) + this->baseDamage ;
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

        ~Player() override = default;

        [[nodiscard]] auto getExperience() const {
            return experience;
        }

        void setExperience(long long int mExperience) {
            experience = mExperience;
        }

        [[nodiscard]] long long int getMoney() const {
            return money;
        }

        void setMoney(long long int mMoney) {
            money = mMoney;
        }

        friend std::ostream & operator<<(std::ostream &os, Player p) {
            os << "Player{health=" << p.healthPoints << "/" << p.getMaxHealth() << ",exp=" << p.experience << ",money=" << p.money << "}";
            return os;
        }

        std::string to_string() {
            std::stringstream out;
            out << "Player{health=" << healthPoints << "/" << getMaxHealth() << ",exp=" << experience << ",money=" << money << "}";
            return out.str();
        }

        Json::Value to_json() {
            Json::Value val;
            val["health"] = this->healthPoints;
            val["strength"] = this->strength;
            val["agility"] = this->agility;
            val["endurance"] = this->endurance;
            val["experience"] = int (this->experience);
            val["money"] = int (this->money);
            return val;
        }

        void from_json(Json::Value value) {
            this->healthPoints = value["health"].asDouble();
            this->strength = value["strength"].asDouble();
            this->agility = value["agility"].asDouble();
            this->endurance = value["endurance"].asDouble();
            this->experience = value["experience"].asInt64();
            this->money = value["money"].asInt64();
        }
    };
}

#endif //CONSOLE_GAME_PLAYER_H
