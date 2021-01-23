//
// Created by Denis Wagner on 1/11/19.
//

#include "Game.hpp"

Game::Game(std::string windowText, float windowWidth, float windowHeight)
    : Application(std::move(windowText), windowWidth, windowHeight)
{
}

void Game::init()
{
    
}

/*
void Game::init()
{
    // default player
    auto nnAILeft = std::make_shared<NNAI>(Side::LEFT);
    auto arrowKeyPlayer = std::make_shared<ArrowKeyPlayer>(Side::RIGHT);
    auto playfieldView = std::make_shared<PlayfieldView>(nnAILeft, arrowKeyPlayer);

    addNode("Playfield", playfieldView);
    addNode("StartScreen", std::make_shared<StartScreenView>(playfieldView));
    addNode("MenuView", std::make_shared<MenuView>());
    addNode("SettingsView", std::make_shared<SettingsView>(playfieldView));
    setNode("StartScreen");
}
*/
