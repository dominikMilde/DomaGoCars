#include <SFGUI/SFGUI.hpp>

#include "NNSimulation.h"
#include "main.h"
#include "config.h"
#include "storage.h"
#include "neuralnetwork.h"

int NNSimulation::Run(sf::RenderWindow& App) {
	initConfig();
	init();

	vector<double> driverNn = readNnDriver();
	simulateNN(App, driverNn);

	return 0;
}