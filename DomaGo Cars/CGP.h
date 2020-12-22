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
    Graph crossAndReturnBestOfThree(Graph firstParent, Graph secondParent);
    double calculateFunction(vector<double> calcOutputs, int functionId, vector<int> nodeInputIds);

public:
    CGP(int numNodeInputs, int numInputs, int numOutputs, int numRows, int numCols);
    vector<int> randomGraph();
    void mutation(vector<int>& graph);
    vector<int> crossover(vector<int>& mainGraph, vector<int>& otherGraph);
    double fitnessFunction(vector<int> graph);
    vector<int> run();
    vector<double> calculateOutputs(vector<double> inputs, vector<int> graph);

};

#endif
