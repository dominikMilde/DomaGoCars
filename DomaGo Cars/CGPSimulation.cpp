#include <SFGUI/SFGUI.hpp>

#include "CGPSimulation.h"
#include "main.h"
#include "config.h"
#include "storage.h"
#include "CGP.h"

int CGPSimulation::Run(sf::RenderWindow& App) {
	initConfig();
	init();

	vector<int> driverCgp = readCgpDriver();
	simulateCGP(App, driverCgp);

	return 0;
}