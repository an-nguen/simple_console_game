//
// Created by an on 8/18/20.
//

#ifndef CONSOLE_GAME_GAME_MENU_H
#define CONSOLE_GAME_GAME_MENU_H

#include <string>
#include <functional>
#include <iostream>
#include <sstream>
#include <memory>
#include <curses.h>

namespace console_game {
    class GameMenuItem {
    protected:
        std::string m_menu_item_name{};
        std::function<void ()> m_function{};
    public:
        GameMenuItem() = default;

        GameMenuItem(const std::string &mMenuItemName, const std::function<void()> &mFunction) : m_menu_item_name(
                mMenuItemName), m_function(mFunction) {}

        virtual ~GameMenuItem() = default;

        [[nodiscard]] std::string getName() const {
            return m_menu_item_name;
        }
        void setName(const std::string& name) {
            this->m_menu_item_name = name;
        }

        virtual void run() {
            m_function();
        }
    };

    template <class T>
    class GameMenu : public GameMenuItem {
    protected:
        std::vector<GameMenuItem> m_menu_items{};
        std::string m_menu_title{};
        T m_menu_preprint{};

    public:
        GameMenu() = default;
        explicit GameMenu(std::initializer_list<GameMenuItem> items) {
            this->m_menu_items.assign(items);
        }
        explicit GameMenu(const std::string &name, const std::string &title, std::initializer_list<GameMenuItem> items) {
            this->m_menu_item_name.assign(name);
            this->m_menu_items.assign(items);
            this->m_menu_title.assign(title);
        }
        virtual ~GameMenu() = default;

        void addItem(const std::string& name, const std::function<void ()>& action) {
            GameMenuItem item(name, action);
            this->m_menu_items.push_back(item);
        }

        void addItem(GameMenuItem &item) {
            this->m_menu_items.push_back(item);
        }

        [[nodiscard]] const std::vector<GameMenuItem> &getMenuItems() const {
            return m_menu_items;
        }

        void setMenuItems(const std::vector<GameMenuItem> &mMenuItems) {
            m_menu_items = mMenuItems;
        }

        const std::string &getMenuTitle() const {
            return m_menu_title;
        }

        void setMenuTitle(const std::string &mMenuTitle) {
            m_menu_title = mMenuTitle;
        }

        void setPreprint(T &obj) {
            this->m_menu_preprint = obj;
        }

        std::stringstream print() {
            std::stringstream out;
            if (this->m_menu_preprint != nullptr)
                out << std::endl << *this->m_menu_preprint << std::endl;
            out << m_menu_item_name << ":" << std::endl;
            for (size_t i = 0; i < m_menu_items.size(); i++) {
                out << i << ". " << m_menu_items.at(i).getName() << std::endl;
            }
            return out;
        }

        void run() override {
            initscr();
            halfdelay(100);
            noecho();
            bool retry = false;
            short sel;
            do {
                try {
                    printw(print().str().c_str());
                    char c = getch();
                    sel = c - '0';
                    if (std::cin.fail()) {
                        throw std::runtime_error("wrong input");
                    } else {
                        retry = false;
                    }
                    clear();
                } catch (std::exception &e) {

                    printw("%s\n", e.what());
                    retry = true;
                }
            } while (retry);
            if (sel >= 0 && sel < this->m_menu_items.size()) {
                this->m_menu_items.at(sel).run();
            }
            run();
        }

    };
}

#endif //CONSOLE_GAME_GAME_MENU_H
