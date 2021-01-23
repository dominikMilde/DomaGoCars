#ifndef PONG_GAME_HPP
#define PONG_GAME_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Application.h"

class Game : public Application {
public:
    Game(std::string windowText, float windowWidth, float windowHeight);
    virtual void init() override;
};

#endif //PONG_GAME_HPP