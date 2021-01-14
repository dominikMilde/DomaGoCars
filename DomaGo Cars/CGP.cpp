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

constexpr double MUTATION_PROB = 0.4;
constexpr int POPULATION_SIZE = 10;
constexpr int GENERATIONS = 10;
constexpr int NUM_FUNCTIONS = 4;

int numNodeInputs = 2;
int numInputs = 6;
int numOutputs = 2;
int numRows = 10;
int numCols = 10;
int graphSize = numCols * numRows * (numNodeInputs + 1) + numOutputs;

vector<Graph> graphs;

double fitnessFunction(vector<int> graph)
{
    CGP cgp(numInputs, numOutputs, numRows, numCols, numNodeInputs);
    cgp.graph = graph;
    return 1;
    // return evaluate(cgp);
}

void mutation(Graph& g)
{
    vector<int> graph = g.graph;
    int index = rand() % (graphSize);

    if (index < graphSize - numOutputs)
    {
        if (index % (numNodeInputs + 1) == 0)
        {
            int functionId = rand() % NUM_FUNCTIONS;
            graph[index] = functionId;
        }
        else
        {
            int indOfColumn = index / (numRows * (1 + numNodeInputs));
            int currMaxNodeOut = numInputs + indOfColumn * numRows;
            graph[index] = rand() % currMaxNodeOut;
        }
    }
    else
    {
        int outRandom = rand() % (numInputs + numCols * numRows);
        graph[index] = outRandom;
    }
    g.graph = graph;
    g.fitness = fitnessFunction(graph);
}

vector<int> crossover(vector<int>& mainGraph, vector<int>& otherGraph)
{
    vector<int> child = mainGraph;
    int index = rand() % (numRows * numCols);
    int startOfCut = index * (1 + numNodeInputs);
    for (startOfCut; startOfCut < graphSize; startOfCut++)
    {
        child[startOfCut] = otherGraph[startOfCut];
    }
    return child;
}

Graph crossAndReturnBestOfThree(Graph firstParent, Graph secondParent)
{
    vector<int> crossGraph = crossover(firstParent.graph, secondParent.graph);
    Graph child(crossGraph, fitnessFunction(crossGraph));

    Graph betterParent = firstParent;
    if (secondParent.fitness > firstParent.fitness)
    {
        betterParent = secondParent;
    }

    if (child.fitness > betterParent.fitness)
    {
        return child;
    }
    return betterParent;
}

vector<int> randomGraph()
{
    vector<int> graph;

    for (int i = 0; i < numCols; i++)
    {
        int currMaxNodeOut = numInputs + i * numRows;
        for (int j = 0; j < numRows; j++)
        {
            graph.push_back(rand() % NUM_FUNCTIONS);
            for (int k = 0; k < numNodeInputs; k++)
            {
                graph.push_back(rand() % currMaxNodeOut);
            }
        }
    }

    int maxNodeOut = numInputs + numRows * numCols;
    for (int i = 0; i < numOutputs; i++)
    {
        graph.push_back(rand() % maxNodeOut);
    }
    return graph;
}

void fillInitialPopulationCgp()
{
    for (int i = 0; i < POPULATION_SIZE; i++)
    {
        vector<int> rG = randomGraph();
        //cout << "pop " << i << endl;
        Graph randGraph = Graph(rG, fitnessFunction(rG));
        //cout << "fitness pocetnog: " << fitnessFunction(rG) << endl;
        graphs.push_back(randGraph);
    }
}

double calculateFitness()
{
    double sumFitness = 0.;
    for (int j = 0; j < POPULATION_SIZE; j++)
    {
        double fitness = graphs[0].fitness;
        if (isnan(fitness))
            fitness = 0.;

        graphs[j].fitness = fitness;
        sumFitness += fitness;
    }
    //cout << "izracunao sam fitness" << endl;
    return sumFitness;
}

void runGeneration() {
    double sumFitness = calculateFitness();

    vector<Graph> newGraphs;
    while (newGraphs.size() < POPULATION_SIZE)
    {
        double select1 = sumFitness * static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
        double select2 = sumFitness * static_cast<double>(rand()) / static_cast<double>(RAND_MAX);

        int idx1 = -1;
        int idx2 = -1;

        double currSum = 0.;
        int k = 0;
        while (idx1 == -1 || idx2 == -1) {
            currSum += graphs[k].fitness;
            if (idx1 == -1 && currSum >= select1)
            {
                idx1 = k;
            }
            if (idx2 == -1 && currSum >= select2)
            {
                idx2 = k;
            }
            k++;
        }
        Graph offspring = crossAndReturnBestOfThree(graphs[idx1], graphs[idx2]);

        static bernoulli_distribution choice(MUTATION_PROB);
        if (choice(randomEngineCGP))
        {
            mutation(offspring);
        }

        newGraphs.push_back(offspring);
    }
    graphs = newGraphs;
}

int findBestGraph()
{
    double maxFitness = graphs[0].fitness;
    int idxBest = 0;
    for (int i = 1; i < POPULATION_SIZE; i++) {
        if (graphs[i].fitness > maxFitness) {
            idxBest = i;
        }
    }
    return idxBest;
}

void print(Graph& g) {
    for (int i = 0; i < graphSize - numOutputs; i++)
    {
        if (i % (numNodeInputs + 1) == 0)
            cout << endl;
        cout << g.graph[i] << " ";
    }
    cout << endl << "Output: ";
    for (int i = graphSize - numOutputs; i < graphSize; i++)
    {
        cout << g.graph[i] << " ";
    }

    cout << endl
        << "CGP Fitness: " << g.fitness << endl << endl << endl;
}

void runCGP()
{
    fillInitialPopulationCgp();

    for (int gen = 1; gen <= GENERATIONS; ++gen) {

        cout << "GEN #" << gen << ":" << endl;

        runGeneration();

        int bestGraphIndex = findBestGraph();

        print(graphs[bestGraphIndex]);
    }
}