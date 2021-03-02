#include <SFGUI/SFGUI.hpp>

#include "UserSimulation.h"
#include "main.h"
#include "config.h"
#include "neuralnetwork.h"
#include "storage.h"

int UserSimulation::Run(sf::RenderWindow& App) {
	initConfig();
	initRace();
	simulate(App);

	return 0;
}