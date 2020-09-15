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
#include <utility>
#include <curses.h>

namespace console_game {
    class GameMenuItem {
    protected:
        std::string menuItemName{};
        std::function<void ()> function{};
    public:
        GameMenuItem() = default;

        GameMenuItem(std::string mMenuItemName, std::function<void()> mFunction) : menuItemName(std::move(
                mMenuItemName)), function(std::move(mFunction)) {}

        virtual ~GameMenuItem() = default;

        [[nodiscard]] std::string getName() const {
            return menuItemName;
        }
        void setName(const std::string& name) {
            this->menuItemName = name;
        }

        virtual void run() {
            function();
        }
    };

    template <class T, class U>
    concept Derived = std::is_base_of<U, T>::value;

    template <class T>
    class GameMenu : public GameMenuItem {
    protected:
        std::vector<GameMenuItem> menuItems{};
        std::string menuTitle{};
        T menuPreprint{};
        bool isChild = false;

    public:
        GameMenu() = default;
        GameMenu(std::initializer_list<GameMenuItem> items) {
            this->menuItems.assign(items);
        }
        explicit GameMenu(const std::string &name, const std::string &title, T &preprint, std::initializer_list<GameMenuItem> items) {
            this->menuItemName.assign(name);
            this->menuPreprint = preprint;
            this->menuItems.assign(items);
            this->menuTitle.assign(title);
        }
        ~GameMenu() override = default;

        void addItem(const std::string& name, const std::function<void ()>& action) {
            GameMenuItem item(name, action);
            this->menuItems.push_back(item);
        }

        void addItem(GameMenuItem &item) {
            this->menuItems.push_back(item);
        }

        [[nodiscard]] const std::vector<GameMenuItem> &getMenuItems() const {
            return menuItems;
        }

        void setMenuItems(const std::vector<GameMenuItem> &mMenuItems) {
            menuItems = mMenuItems;
        }

        [[nodiscard]] const std::string &getMenuTitle() const {
            return menuTitle;
        }

        void setMenuTitle(const std::string &mMenuTitle) {
            menuTitle = mMenuTitle;
        }

        void setPreprint(T &obj) {
            this->menuPreprint = obj;
        }

        bool getIsChild() const {
            return isChild;
        }

        void setIsChild(bool val) {
            this->isChild = val;
        }

        std::stringstream print() {
            std::stringstream out;
            if (this->menuPreprint != nullptr)
                out << std::endl << *this->menuPreprint << std::endl;
            out << menuItemName << ":" << std::endl;
            for (size_t i = 0; i < menuItems.size(); i++) {
                out << i << ". " << menuItems.at(i).getName() << std::endl;
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
            bool isBack = false;
            if (sel >= 0 && sel < this->menuItems.size()) {
                if (this->isChild) {
                    if (sel == 0) {
                        isBack = true;
                    } else {
                        this->menuItems.at(sel).run();
                    }
                } else {
                    this->menuItems.at(sel).run();
                }
            }
            if (!isBack)
                run();
        }

    };
}

#endif //CONSOLE_GAME_GAME_MENU_H
