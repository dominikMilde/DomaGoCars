#pragma once
#include "Jedinka.h"
#include <SFML/Graphics.hpp>

void init();
double evaluate(Jedinka* jedinka);
void simulate(sf::RenderWindow& App, Jedinka* jedinka);
void simulate(sf::RenderWindow& App);