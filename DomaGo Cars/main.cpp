#include <iostream>
#include <time.h>
#include <vector>
#include <SFML/Graphics.hpp>
#include "simulator.h"
#include "CGP.h"

using namespace std;

constexpr double FITNESS_KOEF = 2.5;

constexpr int MAX_SIM_TIME = 2.5;
constexpr int MAX_SIM_DIST = 2.5;
constexpr int MAX_EVA_TIME = 2.5;
constexpr int MAX_EVA_DIST = 2.5;

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

vector<sf::Image> images;
sf::Image displayedImage;

sf::CircleShape player(20.0f);
sf::Texture playerTexture;

vector<string> tracks;
string displayedTrack;

void init() {
	// TODO Vilim - Ovdje u�itaj iz config datoteke
	// tracks - imena datoteka staza na kojima jedinke u�e
	// displayedTrack - ime datoteke staze koja se prikazuje
	//tracks = config.fillTracks();
	//displayedTrack = config.getDisplayedTrack();

	// ovdje smo hardkodirali da mozemo isprobat, zanemari ovu inicijalizaciju tracks i displayedTrack

	tracks.push_back("background1.jpg");
	tracks.push_back("background2.jpg");
	tracks.push_back("background3.jpg");
	tracks.push_back("background4.jpg");
	tracks.push_back("background5.jpg");

	displayedTrack = "background5.jpg";

	// dalje ostaje isto

	player.setOrigin(12.0f, 15.0f);
	player.setScale(1.5, 1.2);
	playerTexture.loadFromFile("avatar.jpg");
	player.setTexture(&playerTexture);

	for (int i = 0; i < tracks.size(); i++) {
		sf::Texture texture;
		texture.loadFromFile(tracks.at(i));
		images.push_back(texture.copyToImage());
	}

	backgroundTexture.loadFromFile(displayedTrack);
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

void simulate(Jedinka* jedinka) {

	player.setPosition(imageWidth / 2, imageHeight / 1.2);
	player.setRotation(0);

	sf::Vector2f vector = player.getPosition();
	simulator sim(vector.x, vector.y, displayedImage);
	
	if (jedinka == nullptr)
		sim.setKOEF(1);
	else sim.setKOEF(20);

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

		auto color1 = displayedImage.getPixel(x, y);

		
		if (jedinka == nullptr && color1 == sf::Color::Black || sim.getAngleDistance() > 5000 || sim.getT() > 50000)
		{
			if (sim.getAngleDistance() > MAX_SIM_DIST)
				cout << "(distance exceeded) ";
			else if (sim.getT() > MAX_SIM_TIME)
				cout << "(time expired) ";
			else
				cout << "(crashed) ";
			return;
		}
		else if (color1 == sf::Color::Black) {
			return;
		}
	}
}

void simulate() {
	simulate(nullptr);
}

double evaluate(Jedinka* jedinka)
{
	double fitness = 0;

	for (int i = 0; i < tracks.size(); i++) {
		cout << i + 1 << ": ";
		
		sf::Image image = images.at(i);

		player.setPosition(imageWidth / 2, imageHeight / 1.2);
		player.setRotation(0);

		sf::Vector2f vector = player.getPosition();
		simulator sim(vector.x, vector.y, image);
		sim.setKOEF(10);

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

			if (x < 0 || x > imageWidth || y < 0 || y > imageHeight) {
				fitness += pow(sim.getAngleDistance(), FITNESS_KOEF) / sim.getT();
				break;
			}

			auto color1 = image.getPixel(x, y);

			if (color1 == sf::Color::Black || sim.getAngleDistance() > MAX_EVA_DIST || sim.getT() > MAX_EVA_TIME)
			{
				if (sim.getAngleDistance() > MAX_EVA_DIST)
					cout << "(distance exceeded) ";
				else if (sim.getT() > MAX_EVA_TIME)
					cout << "(time expired) ";
				else
					cout << "(crashed) ";
				fitness += pow(pow(sim.getAngleDistance(), FITNESS_KOEF) / sim.getT(), 0.4);
				break;
			}
		}
	}

	return fitness;
}
