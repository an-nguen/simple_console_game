//
// Created by an on 8/18/20.
//

#ifndef CONSOLE_GAME_ITEM_H
#define CONSOLE_GAME_ITEM_H

#include <utility>

#include "game_obj.h"

namespace console_game {
    class Item : public console_game::GameObj {
    private:
        std::string m_name{};
        std::string m_description{};
        double m_extra_strength = 0;
        double m_extra_agility = 0;
        double m_extra_endurance = 0;
    public:
        Item() = default;
        explicit Item(std::string  mName, std::string  mDescription, double mExtraStrength, double mExtraAgility, double mExtraEndurance)
                : m_name(std::move(mName)),
                  m_description(std::move(mDescription)),
                  m_extra_strength(mExtraStrength),
                  m_extra_agility(mExtraAgility),
                  m_extra_endurance(mExtraEndurance) {}

        [[nodiscard]] const std::string &getName() const {
            return m_name;
        }

        void setName(const std::string &name) {
            Item::m_name = name;
        }

        [[nodiscard]] const std::string &getDescription() const {
            return m_description;
        }

        void setDescription(const std::string &description) {
            Item::m_description = description;
        }

        [[nodiscard]] double getMExtraStrength() const {
            return m_extra_strength;
        }

        void setMExtraStrength(double mExtraStrength) {
            m_extra_strength = mExtraStrength;
        }

        [[nodiscard]] double getMExtraAgility() const {
            return m_extra_agility;
        }

        void setMExtraAgility(double mExtraAgility) {
            m_extra_agility = mExtraAgility;
        }

        [[nodiscard]] double getMExtraEndurance() const {
            return m_extra_endurance;
        }

        void setMExtraEndurance(double mExtraEndurance) {
            m_extra_endurance = mExtraEndurance;
        }
    };
}

#endif //CONSOLE_GAME_ITEM_H
