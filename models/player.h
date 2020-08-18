//
// Created by an on 8/9/20.
//

#ifndef CONSOLE_GAME_PLAYER_H
#define CONSOLE_GAME_PLAYER_H

#include <random>
#include <sstream>

#include "game_obj.h"
#include "item.h"
#include "game_living_obj.h"

namespace console_game {
    class Player : public GameObj, public GameLivingObject {
    private:
        double m_strength = 10;
        double m_agility = 10;
        double m_endurance = 10;
        double m_min_hit_chance = 0;
        double m_max_hit_chance = 1;
        long long m_experience = 0;
        long long m_money = 0;

        std::vector<Item> m_items{};
    public:
        Player() = default;
        explicit Player(double mStrength = 10,
                        double mAgility = 10,
                        double mEndurance = 10)
                : m_strength(mStrength),
                  m_agility(mAgility),
                  m_endurance(mEndurance) {
            this->calcTotalHP();
        }

        void calcTotalHP() {
            this->m_health = this->m_base_health + this->m_endurance * 20;
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

        ~Player() override = default;

        [[nodiscard]] auto getExperience() const {
            return m_experience;
        }

        void setExperience(long long int mExperience) {
            m_experience = mExperience;
        }

        [[nodiscard]] long long int getMoney() const {
            return m_money;
        }

        void setMoney(long long int mMoney) {
            m_money = mMoney;
        }

        friend std::ostream & operator<<(std::ostream &os, const Player &p) {
            os << "Player{health=" << p.m_health << "/" << p.m_max_health << ",exp=" << p.m_experience << ",money=" << p.m_money << "}";
            return os;
        }

        std::string to_string() {
            std::stringstream out;
            out << "Player{health=" << m_health << "/" << m_max_health << ",exp=" << m_experience << ",money=" << m_money << "}";
            return out.str();
        }
    };
}

#endif //CONSOLE_GAME_PLAYER_H
