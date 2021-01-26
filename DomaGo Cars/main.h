#pragma once
#include "Jedinka.h"
#include <SFML/Graphics.hpp>

void init();
void initRace();
double evaluate(Jedinka* jedinka);
void simulate(sf::RenderWindow& App, Jedinka* jedinka);
void simulate(sf::RenderWindow& App);
void simulateRace(sf::RenderWindow& App, Jedinka* jedinka);