#include <SFGUI/SFGUI.hpp>

#include "TwoPlayersSimulation.h"
#include "main.h"
#include "config.h"

int TwoPlayersSimulation::Run(sf::RenderWindow& App) {
	initConfig();
	initRace();
	simulateRace(App, nullptr);

	return 0;
}