#include "TreeStructure.h"

Node* run(vector<double> outputs, vector<double> inputs);
void print(Node* tree, int depth);
void printRes(Node* tree, vector<double> inputs, vector<double> outputs);
void crossover(Node* mainTree, Node* otherTree);
void mutation(Node** tree, int depth);
double fitnessFunction(Node* tree, vector<double> outputs, vector<double> inputs);
Node* randomTree(int depth);
Node* randomNode(int depth);
void del(Node* tree);
bool treeSort(Node* tree1, Node* tree2);