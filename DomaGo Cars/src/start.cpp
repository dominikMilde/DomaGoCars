#include <iostream>
#include <SFML/Graphics.hpp>
#include "screens.h"

int main(int argc, char** argv)
{
	//Applications variables
	std::vector<Screen*> Screens;
	int screen = 0;

	//Window creation
	sf::RenderWindow App(sf::VideoMode(1152, 648, 32), "DomaGo Cars", sf::Style::Titlebar | sf::Style::Close);

	//Screens preparations
	MainMenu mainmenu;
	Screens.push_back(&mainmenu); // 0
	UserSimulation us;
	Screens.push_back(&us); // 1
	NNLearning nnl;
	Screens.push_back(&nnl); // 2
	CGPLearning cgpl;
	Screens.push_back(&cgpl); // 3
	NNSimulation nns;
	Screens.push_back(&nns); // 4
	CGPSimulation cgps;
	Screens.push_back(&cgps); // 5
	PlayMenu playmenu;
	Screens.push_back(&playmenu); // 6
	AIMenu aimenu;
	Screens.push_back(&aimenu); // 7
	TwoPlayersSimulation two_player_simulation;
	Screens.push_back(&two_player_simulation); // 8
	UserVsCGPSimulation us_cgp;
	Screens.push_back(&us_cgp); // 9
	UserVsNNSimulation us_nn;
	Screens.push_back(&us_nn); // 10

	//Main loop
	while (screen >= 0)
	{
		screen = Screens[screen]->Run(App);
	}

	return EXIT_SUCCESS;
}
