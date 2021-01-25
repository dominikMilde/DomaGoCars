#pragma once
#include "Jedinka.h"
#include "Screen.h"
#include "simulator.h"

class Main : public Screen {
private:
	int imageWidth;
	int imageHeight;
	sf::RectangleShape background;
	sf::Texture backgroundTexture;

	vector<sf::Image> images;
	sf::Image displayedImage;

	sf::CircleShape player;
	sf::Texture playerTexture;

	Jedinka* jedinka;

public:
	Main() : imageWidth(1152), imageHeight(648), background(sf::Vector2f(imageWidth, imageHeight)), player(20.0f), jedinka(nullptr) {}
	Main(Jedinka* jedinka) : Main() {
		this->jedinka = jedinka;
	}
	virtual int Run(sf::RenderWindow& App);
	void init();
	double evaluate(Jedinka* jedinka);
	void simulate(Jedinka* jedinka);
	void simulate();
	void setNNJedinka();
	void setCGPJedinka();
	void resetJedinka();
};