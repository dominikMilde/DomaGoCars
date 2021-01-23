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
int main() {
	
	initConfig();

	// inicijalizira simulator
	init();

	vector<double> nn;
	vector<int> cgp;
	vector<double> driverNn;
	vector<int> driverCgp;
	string s;

	switch (globalConfig.akcija) {
	case 0:
		nn = runNN();
		storeNnDriver(nn);
		cout << "Learning finished. Do you want to simulate learned driver? (Y/N) ";
		cin >> s;
		if (s.compare("Y") == 0) {
			driverNn = readNnDriver();
			simulateNN(driverNn);
		}
		else if (s.compare("N") != 0) {
			cout << "Unexpected input." << endl;
		}
		break;
	case 1:
		cgp = runCGP();
		storeCgpDriver(cgp);
		cout << "Learning finished. Do you want to simulate learned driver? (Y/N) ";
		cin >> s;
		if (s.compare("Y") == 0) {
			driverCgp = readCgpDriver();
			simulateCGP(driverCgp);
		}
		else if (s.compare("N") != 0) {
			cout << "Unexpected input." << endl;
		}
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
		while(true) simulate();
	}

	return 0;
}
*/