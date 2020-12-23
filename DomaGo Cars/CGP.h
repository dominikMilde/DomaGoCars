#ifndef DEF_CGP
#define DEF_CGP 1

#include <vector>

using namespace std;

class Graph
{
public:
    vector<int> graph;
    double fitness;
    Graph(vector<int> graph, double fitness);
};

class CGP
{
private:
    int numNodeInputs;
    int numInputs;
    int numOutputs;
    int numRows;
    int numCols;
    double calculateFunction(vector<double> calcOutputs, int functionId, vector<int> nodeInputIds);

public:
    vector<Graph> graphs;
    CGP(int numNodeInputs, int numInputs, int numOutputs, int numRows, int numCols);
    vector<int> randomGraph();
    void mutation(Graph& graph);
    vector<int> crossover(vector<int>& mainGraph, vector<int>& otherGraph);
    double fitnessFunction(vector<int> graph);
    vector<int> run();
    vector<double> calculateOutputs(vector<double> inputs, vector<int> graph);
    void fillPopulation();
    double calculateFitness();
    Graph crossAndReturnBestOfThree(Graph firstParent, Graph secondParent);
    Graph findBest();
    //int main();
};

#endif
