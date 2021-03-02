#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
#include <time.h>
#include <math.h>

#include "CGP.h"
#include "main.h"

using namespace std;

minstd_rand randomEngineCGP;

vector<Graph> graphs;
Graph bestGraph;

double fitnessFunction(vector<int> graph)
{
    CGP cgp(cgpConfig.numInputs, cgpConfig.numOutputs, cgpConfig.numRows, cgpConfig.numCols, cgpConfig.numNodeInputs);
    cgp.graph = graph;
    return evaluate(&cgp);
}

void findBestGraph()
{
    shuffle(graphs.begin(), graphs.end(), randomEngineCGP);

    int idxBest = 0;
    for (int i = 1; i < cgpConfig.populationSize; i++) {
        if (graphs.at(i).fitness > graphs.at(idxBest).fitness) {
            idxBest = i;
        }
    }
    bestGraph = graphs.at(idxBest);
}

Graph mutation()
{
    vector<int> graph = bestGraph.graph;

    int numMutations = round(cgpConfig.graphSize / cgpConfig.mutationRate);

    for (int i = 0; i < numMutations; i++) {
        int index = rand() % (cgpConfig.graphSize);

        if (index < cgpConfig.graphSize - cgpConfig.numOutputs)
        {
            if (index % (cgpConfig.numNodeInputs + 1) == 0)
            {
                int functionId = rand() % cgpConfig.numFunctions;
                graph[index] = functionId;
            }
            else
            {
                int indOfColumn = index / (cgpConfig.numRows * (1 + cgpConfig.numNodeInputs));
                int currMaxNodeOut = cgpConfig.numInputs + indOfColumn * cgpConfig.numRows;
                graph[index] = rand() % currMaxNodeOut;
            }
        }
        else
        {
            int outRandom = rand() % (cgpConfig.numInputs + cgpConfig.numCols * cgpConfig.numRows);
            graph[index] = outRandom;
        }
    }

    double fitness = fitnessFunction(graph);
    Graph mutatedGraph(graph, fitness);

    return mutatedGraph;
}

vector<int> randomGraph()
{
    vector<int> graph;

    for (int i = 0; i < cgpConfig.numCols; i++)
    {
        int currMaxNodeOut = cgpConfig.numInputs + i * cgpConfig.numRows;
        for (int j = 0; j < cgpConfig.numRows; j++)
        {
            graph.push_back(rand() % cgpConfig.numFunctions);
            for (int k = 0; k < cgpConfig.numNodeInputs; k++)
            {
                graph.push_back(rand() % currMaxNodeOut);
            }
        }
    }

    int maxNodeOut = cgpConfig.numInputs + cgpConfig.numRows * cgpConfig.numCols;
    for (int i = 0; i < cgpConfig.numOutputs; i++)
    {
        graph.push_back(rand() % maxNodeOut);
    }
    return graph;
}

void fillInitialPopulationCGP()
{
    graphs.clear();
    graphs.reserve(cgpConfig.populationSize);

    cout << "Generating initial population..." << endl;
    for (int i = 1; i <= cgpConfig.populationSize; i++)
    {
        vector<int> rG = randomGraph();
        cout << "pop #" << i << ": ";
        double fitness = fitnessFunction(rG);
        cout << "fitness: " << fitness << endl;
        Graph randGraph = Graph(rG, fitness);
        graphs.push_back(randGraph);
    }

    findBestGraph();
}

void print(Graph& g) {
    for (int i = 0; i < cgpConfig.graphSize - cgpConfig.numOutputs; i++)
    {
        if (i % (cgpConfig.numNodeInputs + 1) == 0)
            cout << endl;
        cout << g.graph[i] << " ";
    }
    cout << endl << "Output: ";
    for (int i = cgpConfig.graphSize - cgpConfig.numOutputs; i < cgpConfig.graphSize; i++)
    {
        cout << g.graph[i] << " ";
    }

    cout << endl
        << "CGP Fitness: " << g.fitness << endl << endl << endl;
}

void runGeneration() {
    graphs.clear();
    graphs.reserve(cgpConfig.populationSize);

    graphs.push_back(bestGraph);

    for (int i = 1; i < cgpConfig.populationSize; i++)
    {
        cout << "pop " << i << ": ";

        Graph child = mutation();
        graphs.push_back(child);

        cout << "fitness: " << child.fitness << endl;
    }

    findBestGraph();
}

void simulateCGP(sf::RenderWindow& App, vector<int>& graph) {
    CGP cgp(cgpConfig.numInputs, cgpConfig.numOutputs, cgpConfig.numRows, cgpConfig.numCols, cgpConfig.numNodeInputs);
    cgp.graph = graph;
    simulate(App, &cgp);
}

void raceCGP(sf::RenderWindow& App, vector<int>& graph) {
    CGP cgp(cgpConfig.numInputs, cgpConfig.numOutputs, cgpConfig.numRows, cgpConfig.numCols, cgpConfig.numNodeInputs);
    cgp.graph = graph;
    simulateRace(App, &cgp);
}

vector<int> runCGP()
{
    cout << "Started CGP learning" << endl
        << "Number of generations: " << cgpConfig.generations << endl
        << "Population size: " << cgpConfig.populationSize << endl << endl;

    randomEngineCGP.seed(time(nullptr));
    srand(time(0));

    fillInitialPopulationCGP();
    cout << "Fitness najbolje jedinke: " << bestGraph.fitness << endl;

    for (int gen = 1; gen <= cgpConfig.generations; ++gen) {

        cout << endl << "========================================" << endl << endl;
        cout << "GEN #" << gen << endl;

        runGeneration();
        cout << "Fitness najbolje jedinke: " << bestGraph.fitness << endl;
    }

    cout << endl << endl;

    CGP cgp(cgpConfig.numInputs, cgpConfig.numOutputs, cgpConfig.numRows, cgpConfig.numCols, cgpConfig.numNodeInputs);
    cgp.graph = bestGraph.graph;
    return cgp.graph;
}
