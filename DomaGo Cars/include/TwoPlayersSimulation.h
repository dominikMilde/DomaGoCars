#ifndef TWOPLAYERSSIMULATION_H
#define TWOPLAYERSSIMULATION_H

#include "Screen.h"

class TwoPlayersSimulation : public Screen {
public:
	virtual int Run(sf::RenderWindow& App);
};

#endif