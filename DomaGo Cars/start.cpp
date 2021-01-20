#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "main.h"
#include "neuralnetwork.h"
#include "CGP.h"
#include "storage.h"

using namespace std;

/*
* 0 - pohrani nn vozaca
* 1 - pohran cgp vozaca
* 2 - ucitaj nn vozaca
* 3 - ucitaj cgp vozaca
* 4 - korisnik vozi
*/
int akcija = 1;

int main() {
	
	initConfig();

	// inicijalizira simulator
	init();

	vector<double> nn;
	vector<int> cgp;
	vector<double> driverNn;
	vector<int> driverCgp;

	switch (akcija) {
	case 0:
		nn = runNN();
		storeNnDriver(nn);
		break;
	case 1:
		cgp = runCGP();
		storeCgpDriver(cgp);
		break;
	case 2:
		driverNn = readNnDriver();
		simulateNN(driverNn);
		break;
	case 3:
		driverCgp = readCgpDriver();
		simulateCGP(driverCgp);
		break;
	case 4:
		simulate();
	}

	// pokretanje uèenja pomoæu NN-a
	/*vector<double> nn = runNN();

	storeNnDriver(nn);
	vector<double> driver = readNnDriver();
	simulateNN(driver);*/

	// pokretanje uèenja pomoæu CGP-a
	/*CGP best = runCGP();
	storeCgpDriver(best.graph);
	vector<int> driver = readCgpDriver();
	simulateCGP(driver);*/

	// simulacija autica - korisnik vozac
	//simulate();

	return 0;
}
