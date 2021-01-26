#include <SFGUI/SFGUI.hpp>

#include "UserSimulation.h"
#include "main.h"
#include "config.h"

int UserSimulation::Run(sf::RenderWindow& App) {
	initConfig();
	init();
	simulate(App);

	return 0;
}