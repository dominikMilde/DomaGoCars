#include <SFGUI/SFGUI.hpp>

#include "UserVsNNSimulation.h"
#include "main.h"
#include "config.h"
#include "neuralnetwork.h"
#include "storage.h"

int UserVsNNSimulation::Run(sf::RenderWindow& App) {
	initConfig();
	initRace();
	vector<double> driverNn = readNnDriver();
	raceNN(App, driverNn);

	return 0;
}