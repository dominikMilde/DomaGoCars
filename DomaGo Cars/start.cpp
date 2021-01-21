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
int akcija = 2;

int main() {
	
	initConfig();

	// inicijalizira simulator
	init();

	vector<double> nn;
	vector<int> cgp;
	vector<double> driverNn;
	vector<int> driverCgp;
	string s;

	switch (akcija) {
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
		simulate();
	}

	return 0;
}
