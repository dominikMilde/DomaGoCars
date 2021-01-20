#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "main.h"
#include "neuralnetwork.h"
#include "CGP.h"
#include "storage.h"

using namespace std;

int main() {
	
	initConfig();

	// inicijalizira simulator
	init();

	// pokretanje uèenja pomoæu NN-a
	vector<double> nn = runNN();

	storeNnDriver(nn);
	vector<double> driver = readNnDriver();
	simulateNN(driver);

	// pokretanje uèenja pomoæu CGP-a
	/*CGP best = runCGP();
	storeCgpDriver(best.graph);
	vector<int> driver = readCgpDriver();
	simulateCGP(driver);*/

	// simulacija autica - korisnik vozac
	//simulate();

	return 0;
}
