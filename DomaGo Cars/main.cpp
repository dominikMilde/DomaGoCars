#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

#include "simulator.h"
#include "neuralnetwork.h"
#include "CGP.h"
#include "config.h"

using namespace std;

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
sf::RectangleShape background(sf::Vector2f(imageWidth, imageHeight));
sf::Texture backgroundTexture;

vector<sf::Image> images;
sf::Image displayedImage;

sf::CircleShape player(10.0f);
sf::Texture playerTexture;

sf::CircleShape player2(10.0f);
sf::Texture playerTexture2;

void init() {

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

void initRace() {

	player.setOrigin(12.0f, 15.0f);
	player.setScale(1.5, 1.2);
	playerTexture.loadFromFile("avatar.jpg");
	player.setTexture(&playerTexture);

	player2.setOrigin(12.0f, 15.0f);
	player2.setScale(1.5, 1.2);
	playerTexture2.loadFromFile("avatar2.jpg");
	player2.setTexture(&playerTexture2);

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

void userDriver2(simulator& sim) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
	{
		sim.rotateLeft();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
	{
		sim.rotateRight();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
	{
		sim.gas();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
	{
		sim.brake();
	}
	else
	{
		sim.idle();
	}
}

void calculateCrashReturn(sf::CircleShape& player, simulator& sim) {
	sim.calculateCrashReturn();

	player.setPosition(sim.getX(), sim.getY());
	player.setRotation(sim.getAngle() * -1.0);
}

void simulate(sf::RenderWindow& window, Jedinka* jedinka) {

	player.setPosition(imageWidth / 2, imageHeight / 1.2);
	player.setRotation(0);

	sf::Vector2f vector = player.getPosition();
	simulator sim(vector.x, vector.y, displayedImage);

	bool isUser = jedinka == nullptr;

	if (isUser)
		sim.setKOEF(100);
	else sim.setKOEF(100);

	window.setFramerateLimit(60);

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

		if (isUser) {
			userDriver(sim);
		}
		else {
			int akcija = run(sim, jedinka);
			AIDriver(akcija, sim);
		}

		sim.update(!isUser);

		player.setPosition(sim.getX(), sim.getY());
		player.setRotation(sim.getAngle() * -1.0);

		if (sim.getT() % 20 == 0 || true) {
			window.clear();
			window.draw(background);
			window.draw(player);
			window.display();
		}

		float x = player.getPosition().x;
		float y = player.getPosition().y;

		auto color1 = displayedImage.getPixel(x, y);

		if (isUser) {
			if (color1 == sf::Color::Black)
			{
				return;
			}
		}
		else if (!isUser) {
			if (color1 == sf::Color::Black || sim.getAngleDistance() > globalConfig.maxSimDist || sim.getScaledT() > globalConfig.maxSimTime)
			{
				return;
			}
		}
	}
}

void simulateRace(sf::RenderWindow& window, Jedinka* jedinka) {
	bool is_user = jedinka == nullptr;

	cout << endl << "===== PUT CAKLINE =====" << endl << "Zubni Karijes vs Desni" << endl << endl;

	player.setPosition(imageWidth / 2, imageHeight / 1.2 + -10);
	player.setRotation(0);

	player2.setPosition(imageWidth / 2, imageHeight / 1.2 + 45);
	player2.setRotation(0);

	sf::Vector2f vector1 = player.getPosition();
	simulator sim1(vector1.x, vector1.y, displayedImage);

	sf::Vector2f vector2 = player2.getPosition();
	simulator sim2(vector2.x, vector2.y, displayedImage);

	sim1.setKOEF(100);
	sim2.setKOEF(100);

	window.setFramerateLimit(60);

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

		userDriver(sim1);
		if (is_user) {
			userDriver2(sim2);
		}
		else {
			int action = run(sim2, jedinka);
			AIDriver(action, sim2);
		}

		sim1.update(false);
		sim2.update(false);

		player.setPosition(sim1.getX(), sim1.getY());
		player.setRotation(sim1.getAngle() * -1.0);

		player2.setPosition(sim2.getX(), sim2.getY());
		player2.setRotation(sim2.getAngle() * -1.0);

		if (sim1.getT() % 20 == 0 || true) {
			window.clear();
			window.draw(background);
			window.draw(player);
			window.draw(player2);
			window.display();
		}

		float x1 = player.getPosition().x;
		float y1 = player.getPosition().y;

		float x2 = player2.getPosition().x;
		float y2 = player2.getPosition().y;

		auto color1 = displayedImage.getPixel(x1, y1);
		auto color2 = displayedImage.getPixel(x2, y2);

		if (color1 == sf::Color::Black)
		{
			cout << "Alo zubni karijes, ne zabijaj se :(" << endl;
			calculateCrashReturn(player, sim1);
		}

		if (color2 == sf::Color::Black)
		{
			cout << "Alo desni, ne zabijaj se :(" << endl;
			calculateCrashReturn(player2, sim2);
		}

		if (sim1.getAngleDistance() > 1080 || sim2.getAngleDistance() > 1080)
		{
			if (sim1.getAngleDistance() > sim2.getAngleDistance()) {
				cout << "Cestitam zubni karijes, pobjedio/la si!" << endl;
			}
			else {
				cout << "Cestitam desni, pobjedio/la si!" << endl;
			}
			return;
		}
	}
}

void simulate(sf::RenderWindow& App) {
	simulate(App, nullptr);
}

double fitnessMeanFunc(double x) {
	return pow(x, 0.4);
}

double fitnessMeanFuncInv(double x) {
	return pow(x, 1.25);
}

double evaluate(Jedinka* jedinka)
{
	double fitness = 0;

	for (int i = 0; i < globalConfig.tracks.size(); i++) {
		cout << i + 1 << ": ";

		sf::Image image = images.at(i);

		player.setPosition(imageWidth / 2, imageHeight / 1.2);
		player.setRotation(0);

		sf::Vector2f vector = player.getPosition();
		simulator sim(vector.x, vector.y, image);
		sim.setKOEF(100);

		while (true)
		{
			int akcija = run(sim, jedinka);
			AIDriver(akcija, sim);

			sim.update(true);

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
