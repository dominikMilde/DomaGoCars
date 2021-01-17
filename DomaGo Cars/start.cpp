#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "main.h"
#include "neuralnetwork.h"
#include "CGP.h"
#include "storage.h"

using namespace std;

int nodeInputs = 2;
int inputs = 6;
int outputs = 2;
int rows = 10;
int cols = 10;


int main() {
	
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
