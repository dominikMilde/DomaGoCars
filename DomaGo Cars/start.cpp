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
	cout << "Inicijalizirao sam config" << endl;

	cout << globalConfig.maxEvaTime << " " << globalConfig.rr << " " << cgpConfig.breakpoint << " " << neuralNetworkConfig.generations << endl;

	// inicijalizira simulator
	init();
	cout << "Inicijalizirao sam sim" << endl;

	// pokretanje u�enja pomo�u NN-a
	cout << "Pokrecem NN" << endl;
	vector<double> nn = runNN();
	cout << "Zavrsavam NN" << endl;
	storeNnDriver(nn);
	vector<double> driver = readNnDriver();
	simulateNN(driver);

	// pokretanje u�enja pomo�u CGP-a
	/*CGP best = runCGP();
	storeCgpDriver(best.graph);
	vector<int> driver = readCgpDriver();
	simulateCGP(driver);*/

	// simulacija autica - korisnik vozac
	//simulate();

	return 0;
}
