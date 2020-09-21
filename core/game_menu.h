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
    struct MenuInterface {
        virtual const std::string &getName() = 0;
        virtual void run() = 0;
    };

    template <class T, class U>
    concept Derived = std::is_base_of<U, T>::value;

    class MenuItem : public MenuInterface {
    protected:
        std::string menuTitle{};
        std::function<void ()> func{};
    public:
    private:
        const std::string &getName() override {
            return menuTitle;
        }

    public:
        MenuItem() = default;
        explicit MenuItem(std::string name, std::function<void ()> f) :
         menuTitle(std::move(name)), func(std::move(f)) {}
        void run() override {
            this->func();
        }
    };

    template <class T>
    class GameMenu : public MenuInterface {
    protected:
        std::vector<MenuInterface *> menuItems{};
        std::string menuTitle{};
        std::string menuItemName{};
        T menuPreprint{};
        bool isChild = false;

    public:
        GameMenu() = default;
        explicit GameMenu(const std::string &name, const std::string &title, T &preprint, std::initializer_list<MenuInterface *> items) {
            this->menuItemName.assign(name);
            this->menuPreprint = preprint;
            this->menuItems = items;
            this->menuTitle.assign(title);
        }
        virtual ~GameMenu() {

            for ( auto i = std::begin(menuItems); i != std::end(menuItems);) {
                i = menuItems.erase(i);
            }
        }

        void addItem(const std::string& name, const std::function<void ()>& action) {
            auto *item = new MenuItem(name, action);
            this->menuItems.push_back(item);
        }

        void addItem(MenuInterface * item) {
            this->menuItems.push_back(item);
        }

        [[nodiscard]] const std::vector<MenuInterface *> &getMenuItems() const {
            return menuItems;
        }

        void setMenuItems(const std::vector<MenuInterface *> &mMenuItems) {
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

        const std::string &getName() override {
            return menuItemName;
        }

        std::stringstream print() {
            std::stringstream out;
            if (this->menuPreprint != nullptr)
                out << std::endl << *this->menuPreprint << std::endl;
            out << menuItemName << ":" << std::endl;
            for (size_t i = 0; i < menuItems.size(); i++) {
                out << i << ". " << menuItems.at(i)->getName() << std::endl;
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
                        this->menuItems.at(sel)->run();
                    }
                } else {
                    this->menuItems.at(sel)->run();
                }
            }
            if (!isBack)
                run();
        }

    };
}

#endif //CONSOLE_GAME_GAME_MENU_H
