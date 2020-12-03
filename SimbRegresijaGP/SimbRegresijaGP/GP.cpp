#include <iostream>
#include <vector>
#include <algorithm>
#include "TreeStructure.h"
#include "GP.h"

using namespace std;

const int TREE_DEPTH = 4;
const double MUT_RATE = 0.1;
const int POPULATION_SIZE = 400;
const int GENERATIONS = 10;
const int DIED = 120;

void crossover(Node* mainTree, Node* otherTree) {

	int mainChildren = mainTree->numOfChildren;
	int otherChildren = otherTree->numOfChildren;

	if (mainChildren > 0 && otherChildren > 0) {
		int mainChild = rand() % mainChildren;
		int otherChild = rand() % otherChildren;
		del(mainTree->children.at(mainChild));
		mainTree->children.at(mainChild) = (otherTree->children.at(otherChild))->copy();
	}
}

void mutation(Node** tree, int depth) {

	bool shouldMutate = ((double)rand() / RAND_MAX) < MUT_RATE;

	if (shouldMutate) {
		Node* newNode = randomNode(depth + 1);
		for (int i = 0; i < newNode->numOfChildren; i++) {
			newNode->children.at(i) = randomTree(depth + 1);
		}
		free(*tree);
		*tree = newNode;
	}

	for (int i = 0; i < (*tree)->numOfChildren; i++) {
		mutation(&(*tree)->children.at(i), depth + 1);
	}
}

double fitnessFunction(Node* tree, vector<double> outputs, vector<double> inputs) {
	double sum = 0.;
	for (int i = 0; i < outputs.size(); i++) {
		sum += abs(outputs.at(i) - tree->calculate(inputs.at(i)));
	}
	return sum;
}

Node* run(vector<double> outputs, vector<double> inputs) {

	Node* trees[POPULATION_SIZE];
	for (int i = 0; i < POPULATION_SIZE; i++) {
		trees[i] = randomTree(0);
	}

	for (int i = 0; i < GENERATIONS; i++) {
		for (int j = 0; j < POPULATION_SIZE; j++) {
			double fitness = fitnessFunction(trees[j], outputs, inputs);
			if (fitness == 0) {
				return trees[j];
			}
			if (isnan(fitness)) fitness = 1E+37;
			trees[j]->fitness = fitness;
		}
		sort(trees, trees + POPULATION_SIZE, treeSort);

		for (int j = 0; j < DIED; j++) {
			del(trees[j]);
			int idx = (rand() % (POPULATION_SIZE - DIED)) + DIED;
			trees[j] = trees[idx]->copy();
			crossover(trees[j], trees[POPULATION_SIZE - 1]);
			mutation(&trees[j], 0);
		}
	}
	return trees[POPULATION_SIZE - 1];
}

void print(Node* tree, int depth) {
	for (int i = 0; i < depth; i++) {
		printf("  ");
	}
	printf("%s\n", tree->name.c_str());
	for (int i = 0; i < tree->numOfChildren; i++) {
		print(tree->children.at(i), depth + 1);
	}
}

Node* randomTree(int depth) {
	Node* tree = randomNode(depth);
	for (int i = 0; i < tree->numOfChildren; i++) {
		tree->children.at(i) = randomNode(depth + 1);
	}
	return tree;
}

Node* randomNode(int depth) {
	Node* node = NULL;
	int n;

	if (depth < TREE_DEPTH) {
		n = rand() % 6;
	}
	else {
		n = rand() % 2;
	}

	if (n == 0) {
		node = new Input();
	}
	else if (n == 1) {
		node = new Constant();
	}
	else if (n == 2) {
		node = new Add();
	}
	else if (n == 3) {
		node = new Sub();
	}
	else if (n == 4) {
		node = new Mul();
	}
	else if (n == 5) {
		node = new Div();
	}

	return node;
}

void del(Node* tree) {
	for (int i = 0; i < tree->numOfChildren; i++) {
		del(tree->children.at(i));
	}
	free(tree);
}

bool treeSort(Node* tree1, Node* tree2) {
	return tree1->fitness > tree2->fitness;
}
