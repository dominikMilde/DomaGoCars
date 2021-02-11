#include <SFGUI/SFGUI.hpp>

#include "UserVsCGPSimulation.h"
#include "main.h"
#include "config.h"
#include "CGP.h"
#include "storage.h"

int UserVsCGPSimulation::Run(sf::RenderWindow& App) {
	initConfig();
	initRace();
	vector<int> driverCgp = readCgpDriver();
	raceCGP(App, driverCgp);

	return 0;
}