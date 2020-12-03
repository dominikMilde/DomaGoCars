#include <iostream>
#include <cmath>
#include <time.h>
#include <vector>
#include "GP.h"
#include "TreeStructure.h"

using namespace std;

const int POPULATION_SIZE = 400;
const int GENERATIONS = 10;
const int DIED = 120;


int main() {
	srand(time(NULL));

	vector<double> inputs = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	vector<double> outputs = { 2, 8, 18, 32, 50, 72, 98, 128, 162, 200 }; //2 * x * x

	Node* tree = run(outputs, inputs);
	print(tree, 0);

	return 0;

}
