#include <iostream>
#include <time.h>
#include <vector>
#include <SFML/Graphics.hpp>
#include "simulator.h"
#include "neuralnetwork.h"
#include "CGP.h"
#include "Jedinka.h"
#include "config.h"
#include "storage.h"
#include "main.h"

using namespace std;

int run(simulator& sim, Jedinka* jedinka) {
	vector<int> inputs(6);
	inputs[0] = sim.getV();
	inputs[1] = sim.getTopDistance();
	cout << "top dstnc" << sim.getTopDistance() << endl;
	inputs[2] = sim.getTopRightDistance();
	inputs[3] = sim.getTopLeftDistance();
	inputs[4] = sim.getRightDistance();
	inputs[5] = sim.getLeftDistance();

	(*jedinka).getClass();
	return (*jedinka).akcija(inputs);
}

void Main::init() {

	player.setOrigin(12.0f, 15.0f);
	player.setScale(1.5, 1.2);
	playerTexture.loadFromFile("avatar.jpg");
	player.setTexture(&playerTexture);

	for (int i = 0; i < globalConfig.tracks.size(); i++) {
		sf::Texture texture;
		texture.loadFromFile(globalConfig.tracks.at(i));
		images.push_back(texture.copyToImage());
	}

	backgroundTexture.loadFromFile(globalConfig.displayedTrack);
	background.setTexture(&backgroundTexture);
	displayedImage = backgroundTexture.copyToImage();
	
}

void AIDriver(int akcija, simulator& sim) {
	if (akcija == 1)
	{
		sim.rotateRight();
	}
	else if (akcija == 2)
	{
		sim.rotateLeft();
	}
	else if (akcija == 3)
	{
		sim.rotateRight();
		sim.brake();
	}
	else if (akcija == 4)
	{
		sim.rotateLeft();
		sim.brake();
	}
	else if (akcija == 5)
	{
		sim.gas();
	}
	else if (akcija == 6)
	{
		sim.rotateLeft();
		sim.gas();
	}
	else if (akcija == 7)
	{
		sim.rotateRight();
		sim.gas();
	}
}

void userDriver(simulator& sim) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		sim.rotateLeft();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		sim.rotateRight();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	{
		sim.gas();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
	{
		sim.brake();
	}
	else
	{
		sim.idle();
	}
}

double fitnessMeanFunc(double x) {
	return pow(x, 0.4);
}

double fitnessMeanFuncInv(double x) {
	return pow(x, 1.25);
}

double Main::evaluate(Jedinka* jedinka)
{
	double fitness = 0;

	for (int i = 0; i < globalConfig.tracks.size(); i++) {
		cout << i + 1 << ": ";

		sf::Image image = images.at(i);

		player.setPosition(imageWidth / 2, imageHeight / 1.2);
		player.setRotation(0);

		sf::Vector2f vector = player.getPosition();
		simulator sim(vector.x, vector.y, image);
		sim.setKOEF(30);

		while (true)
		{
			int akcija = run(sim, jedinka);
			AIDriver(akcija, sim);

			sim.update();

			player.setPosition(sim.getX(), sim.getY());
			player.setRotation(sim.getAngle() * -1.0);

			float x = player.getPosition().x;
			float y = player.getPosition().y;

			if (x < 0 || x > imageWidth || y < 0 || y > imageHeight) {
				fitness += fitnessMeanFunc(pow(sim.getAngleDistance(), globalConfig.fitnessKoef) / sim.getScaledT());
				break;
			}

			auto color1 = image.getPixel(x, y);

			if (color1 == sf::Color::Black || sim.getAngleDistance() > globalConfig.maxEvaDist || sim.getScaledT() > globalConfig.maxEvaTime)
			{
				if (sim.getAngleDistance() > globalConfig.maxEvaDist)
					cout << "(distance exceeded) ";
				else if (sim.getScaledT() > globalConfig.maxEvaTime)
					cout << "(time expired) ";
				else
					cout << "(crashed) ";

				if (sim.getAngleDistance() < 0) {
					fitness += 0;
					break;
				}
				fitness += fitnessMeanFunc(pow(sim.getAngleDistance(), globalConfig.fitnessKoef) / sim.getScaledT());
				break;
			}
		}
	}

	cout << endl;

	return fitnessMeanFuncInv(fitness);
}

int Main::Run(sf::RenderWindow& App) {
	initConfig();
	init();

	while (true) {
		player.setPosition(imageWidth / 2, imageHeight / 1.2);
		player.setRotation(0);

		sf::Vector2f vector = player.getPosition();
		simulator sim(vector.x, vector.y, displayedImage);

		bool isUser = jedinka == nullptr;

		if (isUser)
			sim.setKOEF(1);
		else sim.setKOEF(1);

		while (App.isOpen())
		{
			sf::Event evnt;
			while (App.pollEvent(evnt))
			{
				switch (evnt.type)
				{
				case sf::Event::Closed:
					return -1;
				case sf::Event::Resized:
					std::cout << "New window width: " << evnt.size.width << " New window height: " << evnt.size.height << std::endl;
					break;
				case sf::Event::TextEntered:
					break;
				}
			}

			if (isUser) {
				userDriver(sim);
				cout << sim.getTopDistance() << endl;
			}
			else {
				cout << sim.getTopDistance() << endl;
				int akcija = run(sim, jedinka);
				cout << "A ovdje?6" << endl;
				AIDriver(akcija, sim);
				cout << "A ovdje?7" << endl;
			}

			cout << "A ovdje?5" << endl;

			sim.update();

			player.setPosition(sim.getX(), sim.getY());
			player.setRotation(sim.getAngle() * -1.0);

			if (sim.getT() % 20 == 0) {
				App.clear();
				App.draw(background);
				App.draw(player);
				App.display();
			}

			float x = player.getPosition().x;
			float y = player.getPosition().y;

			auto color1 = displayedImage.getPixel(x, y);

			if (isUser) {
				if (color1 == sf::Color::Black) {
					return 0;
				}
			}
			else if (!isUser) {
				if (color1 == sf::Color::Black || sim.getAngleDistance() > globalConfig.maxSimDist || sim.getScaledT() > globalConfig.maxSimTime)
				{
					return 0;
				}
			}
		}
	}

	//Never reaching this point normally, but just in case, exit the application
	return -1;
}

void Main::setNNJedinka() {
	vector<double> driverNn = readNnDriver();
	*jedinka = createNN(driverNn);
}

void Main::setCGPJedinka() {
	vector<int> driverCgp = readCgpDriver();
	cout << "Usao sam ovdje" << endl;
	CGP cgp = createCGP(driverCgp);
	cout << "I ovdje" << endl;
	jedinka = &cgp;
	cout << "A ovdje?" << endl;
}

void Main::resetJedinka() {
	jedinka = nullptr;
}