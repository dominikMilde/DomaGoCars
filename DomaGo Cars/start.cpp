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
	Screens.push_back(&mainmenu);
	UserSimulation us;
	Screens.push_back(&us);
	NNLearning nnl;
	Screens.push_back(&nnl);
	CGPLearning cgpl;
	Screens.push_back(&cgpl);
	NNSimulation nns;
	Screens.push_back(&nns);
	CGPSimulation cgps;
	Screens.push_back(&cgps);

	//Main loop
	while (screen >= 0)
	{
		screen = Screens[screen]->Run(App);
	}

	return EXIT_SUCCESS;
}
