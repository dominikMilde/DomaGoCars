#include <iostream>
#include "main.h"
#include "neuralnetwork.h"
#include "CGP.h"

int main() {
	
	// inicijalizira simulator
	init();

	// pokretanje uèenja pomoæu NN-a
	//runNN();

	// pokretanje uèenja pomoæu CGP-a
	runCGP();

	// simulacija autica - korisnik vozac
	//simulate();
	
	return 0;
}
