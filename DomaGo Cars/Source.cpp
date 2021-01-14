#include <iostream>
#include "main.h"
#include "neuralnetwork.h"
#include "CGP.h"

int main() {
	init();

	// executeNN();
	// runCGP();

	while (true) {
		evaluate();
	}
	
	return 0;
}
