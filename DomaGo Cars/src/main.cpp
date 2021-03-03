#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

#include "main.h"
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
	playerTexture.loadFromFile("img/avatar.jpg");
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
	playerTexture.loadFromFile("img/avatar.jpg");
	player.setTexture(&playerTexture);

	player2.setOrigin(12.0f, 15.0f);
	player2.setScale(1.5, 1.2);
	playerTexture2.loadFromFile("img/avatar2.jpg");
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

	bool isUser = jedinka == nullptr;
	if (isUser) {
		cout << "===== SINGLE PLAYER =====" << endl << endl;
		cout << "\"Your greatest opponent is yourself.\"" << endl;
		cout << "Try to finish " << globalConfig.numOfLaps << (globalConfig.numOfLaps > 1 ? " laps" : " lap") << " without crashing!" << endl;
		cout << "Press ESC to exit" << endl << endl;
	}
	else {
		cout << "===== AI SIMULATION =====" << endl << endl;
		cout << "Hopefully AI player will finish " << globalConfig.maxSimDist / 360 << (globalConfig.maxSimDist / 360 > 1 ? " laps" : " lap") << " without crashing!" << endl;
		cout << "Press ESC to exit" << endl << endl;
	}
	bool wait = false;
	bool started = false;
	
	player.setPosition(imageWidth / 2, imageHeight / 1.2);
	player.setRotation(0);

	sf::Vector2f vector = player.getPosition();
	simulator sim(vector.x, vector.y, displayedImage);

	if (isUser)
		sim.setKOEF(80);
	else sim.setKOEF(100);

	window.setFramerateLimit(60);

	while (window.isOpen())
	{
		sf::Event evnt;
		while (window.pollEvent(evnt) || wait)
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
			case sf::Event::KeyPressed: {
				if (evnt.key.code == sf::Keyboard::Escape) return;
				if (wait && evnt.key.code == sf::Keyboard::Enter) return simulate(window, jedinka);
				if (isUser && !started && evnt.key.code == sf::Keyboard::Key::W) {
					sim.setT(0);
					started = true;
				}
			}
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

		auto color = displayedImage.getPixel(x, y);

		if (isUser) {
			if (color == sf::Color::Black || sim.getAngleDistance() > globalConfig.numOfLaps * 360)
			{
				if (sim.getAngleDistance() > globalConfig.numOfLaps * 360) {
					cout << "FINISHED!" << endl;
					int time = (int)(sim.getT() * 1.7);
					cout << "Your time : " << time / 100 << ":" << time % 100 << endl << endl;
				}
				else
					cout << "Better luck next time!" << endl << endl;

				wait = true;
				cout << "Press ENTER to restart" << endl;
				cout << "Press ESC to exit" << endl <<endl << endl;
			}
		}
		else if (!isUser) {
			if (color == sf::Color::Black || sim.getAngleDistance() > globalConfig.maxSimDist || sim.getScaledT() > globalConfig.maxSimTime)
			{
				return;
			}
		}
	}
}

void simulateRace(sf::RenderWindow & window, Jedinka * jedinka) {
	bool is_user = jedinka == nullptr;
	bool wait = false;

	cout << "===== TWO PLAYERS =====" << endl << endl;
	cout << "Ferrari vs Mustang" << endl;
	cout << "Try to finish " << globalConfig.numOfLaps << (globalConfig.numOfLaps > 1 ? " laps" : " lap") << " faster than your opponent!" << endl;
	cout << "Press ESC to exit" << endl << endl;

	player.setPosition(imageWidth / 2, imageHeight / 1.2 + -10);
	player.setRotation(0);

	player2.setPosition(imageWidth / 2, imageHeight / 1.2 + 45);
	player2.setRotation(0);

	sf::Vector2f vector1 = player.getPosition();
	simulator sim1(vector1.x, vector1.y, displayedImage);

	sf::Vector2f vector2 = player2.getPosition();
	simulator sim2(vector2.x, vector2.y, displayedImage);

	sim1.setKOEF(80);
	sim2.setKOEF(80);

	window.setFramerateLimit(60);

	while (window.isOpen())
	{
		sf::Event evnt;
		while (window.pollEvent(evnt) || wait)
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
			case sf::Event::KeyPressed: {
				if (evnt.key.code == sf::Keyboard::Escape) return;
				if (wait && evnt.key.code == sf::Keyboard::Enter) return simulateRace(window, jedinka);
			}
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
			cout << "Watch out for those fences, Ferrari!" << endl;
			calculateCrashReturn(player, sim1);
		}

		if (color2 == sf::Color::Black)
		{
			cout << "Get back on track, Mustang!" << endl;
			calculateCrashReturn(player2, sim2);
		}

		if (sim1.getAngleDistance() > globalConfig.numOfLaps * 360 || sim2.getAngleDistance() > globalConfig.numOfLaps * 360)
		{
			if (sim1.getAngleDistance() > sim2.getAngleDistance()) {
				cout << endl << "WINNER: FERRARI" << endl << endl;
			}
			else {
				cout << endl << "WINNER: MUSTANG" << endl << endl;
			}
			wait = true;
			cout << "Press ENTER to restart" << endl;
			cout << "Press ESC to exit" << endl << endl << endl;
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
	bool checked = false;
	double fitness = 0;

	for (int i = 0; i < globalConfig.tracks.size(); i++) {
		cout << i + 1 << ": ";

		sf::Image image = images.at(i);

		player.setPosition(imageWidth / 2, imageHeight / 1.2);
		player.setRotation(0);

		sf::Vector2f vector = player.getPosition();
		simulator sim(vector.x, vector.y, image);
		sim.setKOEF(20);

		while (true)
		{
			int akcija = run(sim, jedinka);
			AIDriver(akcija, sim);

			sim.update(true);

			float x = sim.getX();
			float y = sim.getY();

			if (x < 0 || x > imageWidth || y < 0 || y > imageHeight) {
				fitness += fitnessMeanFunc(pow(sim.getAngleDistance(), globalConfig.fitnessKoef) / sim.getScaledT());
				break;
			}

			player.setPosition(sim.getX(), sim.getY());
			player.setRotation(sim.getAngle() * -1.0);

			auto color = image.getPixel(x, y);

			if (!checked && sim.getScaledT() > globalConfig.maxEvaTime / 10.) {
				if (sim.getAngleDistance() < globalConfig.maxEvaDist / 30.) {
					cout << "(control time expired) ";
					if (sim.getAngleDistance() < 0) {
						fitness += 0;
						break;
					}
					fitness += fitnessMeanFunc(pow(sim.getAngleDistance(), globalConfig.fitnessKoef) / sim.getScaledT());
					break;
				}
				checked = true;
			}

			if (color == sf::Color::Black || sim.getAngleDistance() > globalConfig.maxEvaDist || sim.getScaledT() > globalConfig.maxEvaTime)
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
