#include <iostream>
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include "screens.h"

//#include "neuralnetwork.h"
//#include "CGP.h"
//#include "storage.h"


int main(int argc, char** argv)
{
	//Applications variables
	std::vector<Screen*> Screens;
	int screen = 0;

	//Window creation
	sf::RenderWindow App(sf::VideoMode(1152, 648, 32), "DomaGo Cars", sf::Style::Titlebar | sf::Style::Close);

	//Screens preparations
	MainMenu mainmenu;
	Screens.push_back(&mainmenu);
	Main main;
	Screens.push_back(&main);

	//Main loop
	while (screen >= 0)
	{
		switch (screen) {
		case 1: {
			main.resetJedinka();
			break;
		}
			/*case 2: {
			vector<double> nn = runNN();
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
			ShowWindow(hWnd, SW_SHOW);
			screen = 1;
			break;
		}
		case 3: {

			ShowWindow(hWnd, SW_SHOW);
			screen = 1;
			break;
		}*/
		case 4: {
			main.setNNJedinka();
			screen = 1;
			break;
		}
		case 5: {
			main.setCGPJedinka();
			screen = 1;
			break;
		}
		}
		std::cout << "Screen je " << screen << std::endl;
		screen = Screens[screen]->Run(App);
	}

	return EXIT_SUCCESS;
}

/*
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