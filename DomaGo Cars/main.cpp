#include <iostream>
#include <time.h>
#include <vector>
#include <SFML/Graphics.hpp>
#include "simulator.h"
#include "CGP.h"

using namespace std;

constexpr double FITNESS_KOEF = 2.5;

int run(const simulator& sim, Jedinka* jedinka) {

	vector<int> inputs(6);
	inputs[0] = sim.getV();
	inputs[1] = sim.getTopDistance();
	inputs[2] = sim.getTopRightDistance();
	inputs[3] = sim.getTopLeftDistance();
	inputs[4] = sim.getRightDistance();
	inputs[5] = sim.getLeftDistance();

	return (*jedinka).akcija(inputs);
}

int imageWidth = 1152;
int imageHeight = 648;
sf::RenderWindow window(sf::VideoMode(imageWidth, imageHeight), "SFML auti", sf::Style::Close | sf::Style::Titlebar);
sf::RectangleShape background(sf::Vector2f(imageWidth, imageHeight));
sf::Texture backgroundTexture;
sf::CircleShape player(20.0f);
sf::Image image;
sf::Texture playerTexture;

void init() {
	backgroundTexture.loadFromFile("background.jpg");
	background.setTexture(&backgroundTexture);
	player.setOrigin(12.0f, 15.0f);
	player.setScale(1.5, 1.2);
	playerTexture.loadFromFile("avatar.jpg");
	player.setTexture(&playerTexture);
	image = backgroundTexture.copyToImage();
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

void simulate(Jedinka* jedinka) {
	player.setPosition(imageWidth / 2, imageHeight / 1.2);
	player.setRotation(0);

	sf::Vector2f vector = player.getPosition();
	simulator sim(vector.x, vector.y, image);

	while (window.isOpen())
	{
		sf::Event evnt;
		while (window.pollEvent(evnt))
		{
			switch (evnt.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::Resized:
				std::cout << "New window width: " << evnt.size.width << " New window height: " << evnt.size.height << std::endl;
				break;
			case sf::Event::TextEntered:
				break;
			}
		}

		if (jedinka == nullptr) {
			userDriver(sim);
		}
		else {
			int akcija = run(sim, jedinka);
			AIDriver(akcija, sim);
		}

		sim.update();

		player.setPosition(sim.getX(), sim.getY());
		player.setRotation(sim.getAngle() * -1.0);

		if (sim.getT() % 20 == 0) {
			window.clear();
			window.draw(background);
			window.draw(player);
			window.display();
		}

		float x = player.getPosition().x;
		float y = player.getPosition().y;
		
		//if (sim.getT() % 2000 == 0) sim.print();

		auto color1 = image.getPixel(x, y);

		if (color1 == sf::Color::Black || sim.getAngleDistance() > 10000 || sim.getT() > 200000)
		{
			cout << "Fitness najbolje jedinke: " << pow(sim.getAngleDistance(), FITNESS_KOEF) / sim.getT() << endl;
			return;
		}
	}
}

void simulate() {
	simulate(nullptr);
}

double evaluate(Jedinka* jedinka)
{
	player.setPosition(imageWidth / 2, imageHeight / 1.2);
	player.setRotation(0);

	sf::Vector2f vector = player.getPosition();
	simulator sim(vector.x, vector.y, image);

	while (true)
	{
		int akcija = run(sim, jedinka);
		AIDriver(akcija, sim);

		sim.update();

		player.setPosition(sim.getX(), sim.getY());
		player.setRotation(sim.getAngle() * -1.0);

		float x = player.getPosition().x;
		float y = player.getPosition().y;
		
		/*if (sim.getT() % 2000 == 0)
			cout << sim.getAngle() << " " << sim.getTopDistance() << " " << sim.getLeftDistance() << " " << sim.getRightDistance()
			<< " " << sim.getV() << " " << akcija << " " << sim.getT() << endl;*/

		auto color1 = image.getPixel(x, y);

		if (color1 == sf::Color::Black || sim.getAngleDistance() > 2500 || sim.getT() > 10000)
		{
			//cout << "fitness: " << pow(sim.getAngleDistance(), FITNESS_KOEF) / sim.getT() << endl;
			if (sim.getT() > 30000)
				cout << "(time expired) ";
			return pow(sim.getAngleDistance(), FITNESS_KOEF) / sim.getT();
		}
	}
}
