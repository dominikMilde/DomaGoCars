#pragma once
#include "Screen.h"

class AIMenu : public Screen {
private:
	int action;

public:
	virtual int Run(sf::RenderWindow& App);
	void setAction(int action);
};