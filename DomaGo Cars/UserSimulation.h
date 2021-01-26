#ifndef USERSIMULATION_H
#define USERSIMULATION_H

#include "Screen.h"

class UserSimulation : public Screen {
public: 
	virtual int Run(sf::RenderWindow& App);
};

#endif
