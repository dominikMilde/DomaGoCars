#pragma once

#include "Screen.h"

class NNLearning : public Screen {
private: 
	int action;
	bool replace;
	void learn();
public:
	virtual int Run(sf::RenderWindow& App);
	void setAction(int action);
};