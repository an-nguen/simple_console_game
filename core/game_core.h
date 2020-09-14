//
// Created by an on 21.08.2020.
//

#ifndef CONSOLE_GAME_GAME_CORE_H
#define CONSOLE_GAME_GAME_CORE_H

#include <curses.h>

#include <utility>
#include <filesystem>
#include <fstream>
#include <json/json.h>

#include "game_menu.h"
#include "../models/player.h"
#include "../models/enemy.h"

namespace console_game {
    class GameApplication {
    private:
        std::shared_ptr<Player> mainActor;
    public:
        GameApplication() = default;
        explicit GameApplication(const Player& main_actor) : mainActor(std::make_shared<Player>(main_actor)) {}
        explicit GameApplication(std::shared_ptr<Player> main_actor) : mainActor(std::move(main_actor)) {}
        void gameLoop() {
            std::shared_ptr<console_game::GameMenu<std::shared_ptr<console_game::Player>>> mainMenu(
                    new console_game::GameMenu<std::shared_ptr<console_game::Player>>
                            ("Main menu", "Main menu",{
                                     {"Exit", []() {
                                         exit(0);
                                     }},
                                     {"Go to dungeon", [this, &mainMenu]() {
                                         std::random_device r;
                                         std::default_random_engine e1(r());
                                         std::uniform_int_distribution<int> uniform_dist(1, 9);
                                         auto enemyStrength = uniform_dist(e1);
                                         auto enemyAgility = uniform_dist(e1);
                                         auto enemyEndurance = uniform_dist(e1);
                                         auto enemyBaseDamage = uniform_dist(e1);
                                         std::shared_ptr<Enemy> enemy(
                                                 new console_game::Enemy
                                                         (enemyStrength, enemyAgility, enemyEndurance, enemyBaseDamage));
                                         combatMode(this->mainActor, enemy);
                                     }},
                             }
                            ));
            mainMenu->setPreprint(this->mainActor);
            mainMenu->addItem("Heal (15 money)", [this]() {
                const int cost = 15;
                if (this->mainActor->getMoney() < cost) {
                    printw("Not enough money to heal you.\n");
                } else {
                    printw("You healed.\n");
                    this->mainActor->setMoney(this->mainActor->getMoney() - cost);
                    this->mainActor->setHealth(this->mainActor->getMaxHealth());
                }
            });
            mainMenu->addItem("Save game", [this]() {
                std::filesystem::create_directory("save_files");
                std::ofstream file("save_files/save.sav", std::ofstream::binary | std::ofstream::trunc);
                Json::Value val = this->mainActor->to_json();
                Json::StreamWriterBuilder builder;
                std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
                writer->write(val, &file);
                file.close();
                printw("Game saved!\n");
            });
            mainMenu->addItem("Load game", [this]() {
                Json::Value root;
                std::ifstream file("save_files/save.sav", std::ifstream::binary);
                file >> root;
                printw("The save file loaded.\n");
                this->mainActor->from_json(root);
                file.close();
            });
            mainMenu->run();
            endwin();
        }

        static void combatMode(std::shared_ptr<console_game::Player> &player,
                               std::shared_ptr<console_game::Enemy> &enemy) {
            const auto combat_menu = "Combat:\n1. Attack\n0. Retreat\n";
            double plr_dmg, enemy_dmg;
            bool isRetreat = false;
            while (player->getHealth() > 0 && enemy->getHealth() > 0 && !isRetreat) {
                bool again;
                short usr_sel;
                do {
                    try {
                        clear();
                        printw("%s\n", player->to_string().c_str());
                        printw("%s\n", enemy->toString().c_str());
                        if (enemy_dmg > 0) {
                            printw("Player get damage - %f\n", enemy_dmg);
                        }
                        if (plr_dmg > 0) {
                            printw("Enemy get damage - %f\n", plr_dmg);
                        }
                        printw("%s\n", combat_menu);
                        char c = getch();
                        usr_sel = c - '0';
                        if (std::cin.fail())
                            throw std::runtime_error("wrong input");
                        else again = false;
                    } catch (std::exception &e) {
                        printw("%s\n", e.what());
                        again = true;
                    }
                } while (again);
                switch (usr_sel) {
                    case 1:
                        plr_dmg = player->attack(enemy);
                        enemy_dmg = enemy->attack(player);
                        break;
                    case 0:
                        isRetreat = true;
                        break;
                    default:
                        std::cout << std::endl;
                        break;
                }
            }
            delay_output(1);
            if (player->getHealth() <= 0) {
                printw("You are unconscious.\n");
                player->setHealth(player->getMaxHealth());
                player->setMoney(0);
                player->setExperience(player->getExperience() * 0.5);
            } else if (enemy->getHealth() <= 0) {
                printw("Enemy is unconscious.\n");
                auto get_exp_val = std::abs(enemy->getMaxHealth() * 1.5);
                printw("Earned %f exp points.\n", get_exp_val);
                printw("Earned %f money.\n", get_exp_val * 0.8);
                player->setExperience(player->getExperience() + get_exp_val);
                player->setMoney(player->getMoney() + get_exp_val * 0.8);
            }
        }

        [[nodiscard]] const std::shared_ptr<Player> &getMainActor() const {
            return mainActor;
        }

        void setMainActor(const std::shared_ptr<Player> &mMainActor) {
            mainActor = mMainActor;
        }
    };


}

#endif //CONSOLE_GAME_GAME_CORE_H
