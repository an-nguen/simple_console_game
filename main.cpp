#include <iostream>
#include <memory>
#include <unistd.h>

#include "models/player.h"
#include "models/enemy.h"
#include "models/game_menu.h"


void combatMode(std::shared_ptr<console_game::Player> &player, std::shared_ptr<console_game::Enemy> &enemy) {
    const auto combat_menu = "Combat:\n1. Attack\n0. Retreat\n";
    double plr_dmg, enemy_dmg;
    bool isRetreat = false;
    while (player->getHealth() > 0 && enemy->getHealth() > 0 && !isRetreat) {
        bool again = false;
        short usr_sel;
        do {
            try {
                clear();
                printw("%s\n", player->to_string().c_str());
                printw("%s\n", enemy->to_string().c_str());
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
        auto get_exp_val = std::abs(enemy->getHealth() * 1.5);
        printw("Earned %f exp points.\n", get_exp_val);
        printw("Earned %f money.\n", get_exp_val * 0.8);
        player->setExperience(player->getExperience() + get_exp_val);
        player->setMoney(player->getMoney() + get_exp_val * 0.8);
    }
}

int main() {
    std::shared_ptr<console_game::Player> player(new console_game::Player(10, 10, 10));

    std::shared_ptr<console_game::GameMenu<std::shared_ptr<console_game::Player>>> mainMenu(
            new console_game::GameMenu<std::shared_ptr<console_game::Player>>("Main menu", "Main menu",{
                                               {"Exit", []() {
                                                   exit(0);
                                               }},
                                               {"Go to dungeon", [&player, &mainMenu]() {
                                                   std::random_device r;
                                                   std::default_random_engine e1(r());
                                                   std::uniform_int_distribution<int> uniform_dist(1, 12);
                                                   auto enemy_strength = uniform_dist(e1);
                                                   auto enemy_agility = uniform_dist(e1);
                                                   auto enemy_endurance = uniform_dist(e1);
                                                   auto enemy_base_damage = uniform_dist(e1);
                                                   std::shared_ptr<console_game::Enemy> enemy(new console_game::Enemy(enemy_strength, enemy_agility, enemy_endurance, enemy_base_damage));
                                                   combatMode(player, enemy);
                                               }},
                                       }
            ));
    mainMenu->setPreprint(player);
    mainMenu->addItem("Heal (50 money)", [&player]() {
        if (player->getMoney() < 50) {
            printw("Not enough money to heal you.\n");
        } else {
            printw("You are healed.\n");
            player->setMoney(player->getMoney() - 50);
            player->setHealth(player->getMaxHealth());
        }
    });
    mainMenu->run();
    endwin();
    return 0;
}
