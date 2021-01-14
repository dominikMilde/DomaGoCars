#include <iostream>
#include "main.h"
#include "neuralnetwork.h"
#include "CGP.h"

int main() {
	
	// inicijalizira simulator
	init();

	// pokretanje u�enja pomo�u NN-a
	//runNN();

	// pokretanje u�enja pomo�u CGP-a
	runCGP();

	// simulacija autica - korisnik vozac
	//simulate();
	
	return 0;
}
