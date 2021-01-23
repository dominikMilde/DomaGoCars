#ifndef PONG_MENUVIEW_HPP
#define PONG_MENUVIEW_HPP

#include <SFML/Graphics/Text.hpp>
#include "Node.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFGUI\SFGUI.hpp>
#include <SFGUI\Button.hpp>

class MenuView : public Node {
    sfg::SFGUI sfgui;

    sf::RectangleShape outerBackground;
    sf::RectangleShape innerBackground;
    sf::Text           title;
    sfg::Button        button;

public:
    virtual void init() override;
    virtual void eventHandler(const sf::Event& event) override;
    virtual void draw(sf::RenderWindow& window) override;
};

#endif