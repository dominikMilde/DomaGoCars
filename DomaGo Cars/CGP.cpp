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

double fitnessFunction(vector<int> graph)
{
    CGP cgp(cgpConfig.numInputs, cgpConfig.numOutputs, cgpConfig.numRows, cgpConfig.numCols, cgpConfig.numNodeInputs);
    cgp.graph = graph;
    return evaluate(&cgp);
}

Graph mutation(Graph& g)
{
    vector<int> graph = g.graph;

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

Graph mutateAndChooseBetter(Graph& parent) {
    Graph child = mutation(parent);

    //if (child.fitness > parent.fitness)
        return child;
    //return parent;
}

vector<int> crossover(vector<int>& mainGraph, vector<int>& otherGraph)
{
    vector<int> child = mainGraph;
    int index = rand() % (cgpConfig.numRows * cgpConfig.numCols);
    int startOfCut = index * (1 + cgpConfig.numNodeInputs);
    for (startOfCut; startOfCut < cgpConfig.graphSize; startOfCut++)
    {
        child[startOfCut] = otherGraph[startOfCut];
    }
    return child;
}

Graph crossAndReturnBestOfThree(Graph &firstParent, Graph &secondParent)
{
    vector<int> crossGraph;

    static bernoulli_distribution choice(0.5);
    if (choice(randomEngineCGP))
    {
        crossGraph = crossover(firstParent.graph, secondParent.graph);
    }
    else {
        crossGraph = crossover(secondParent.graph, firstParent.graph);
    }
    
    Graph child(crossGraph, fitnessFunction(crossGraph));

    Graph betterParent = firstParent;
    if (secondParent.fitness > firstParent.fitness)
    {
        betterParent = secondParent;
    }

    //cout << "Zapoceo crossover: fitnessi su: " << firstParent.fitness << " " << secondParent.fitness << " " << child.fitness << endl;

    if (child.fitness > betterParent.fitness)
    {
        return child;
    }
    return betterParent;
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

    graphs.shrink_to_fit();
}

double calculateFitness()
{
    double sumFitness = 0.;
    for (int j = 0; j < cgpConfig.populationSize; j++)
    {
        double fitness = graphs.at(j).fitness;
        if (isnan(fitness))
            fitness = 0.;

        graphs.at(j).fitness = fitness;
        sumFitness += fitness;
    }
    //cout << "izracunao sam fitness" << endl;
    return sumFitness;
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
    double sumFitness = calculateFitness();
    
    vector<Graph> newGraphs;
    int i = 1;

    while (newGraphs.size() < cgpConfig.populationSize)
    {
        cout << "pop " << i++ << ": ";

        double select1 = sumFitness * static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
        double select2 = sumFitness * static_cast<double>(rand()) / static_cast<double>(RAND_MAX);

        int idx1 = -1;
        int idx2 = -1;

        double currSum = 0.;
        int k = 0;
        while (idx1 == -1 || idx2 == -1) {
            currSum += graphs.at(k).fitness;
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
        Graph offspring = crossAndReturnBestOfThree(graphs.at(idx1), graphs.at(idx2));

        static bernoulli_distribution choice(cgpConfig.mutationProb);
        if (choice(randomEngineCGP))
        {
            //cout << "mutirao sam: prije=" << offspring.fitness;
            offspring = mutateAndChooseBetter(offspring);
            //cout << " poslije=" << offspring.fitness << " ";
        }
        cout << "fitness: " << offspring.fitness << endl;

        newGraphs.push_back(offspring);
    }
    graphs = newGraphs;
}

int findBestGraph()
{
    double maxFitness = graphs.at(0).fitness;
    int idxBest = 0;
    for (int i = 1; i < cgpConfig.populationSize; i++) {
        if (graphs.at(i).fitness > maxFitness) {
            idxBest = i;
        }
    }
    return idxBest;
}

void simulateCGP(vector<int> &graph) {
    CGP cgp(cgpConfig.numInputs, cgpConfig.numOutputs, cgpConfig.numRows, cgpConfig.numCols, cgpConfig.numNodeInputs);
    cgp.graph = graph;
    simulate(&cgp);
}

CGP runCGP()
{
    cout << "Started CGP learning" << endl 
        << "Number of generations: " << cgpConfig.generations << endl
        << "Population size: " << cgpConfig.populationSize << endl << endl;
    
    randomEngineCGP.seed(time(nullptr));
    srand(time(0));

    fillInitialPopulationCGP();

    int bestGraphIndex = findBestGraph();
    //simulateCGP(graphs.at(bestGraphIndex).graph);

    for (int gen = 1; gen <= cgpConfig.generations; ++gen) {

        cout << endl << "========================================" << endl << endl;
        cout << "GEN #" << gen << endl;

        runGeneration();

        int bestGraphIndex = findBestGraph();
        //simulateCGP(graphs.at(bestGraphIndex).graph);
        cout << "Fitness najbolje jedinke: " << graphs.at(bestGraphIndex).fitness << endl;

        if (graphs.at(bestGraphIndex).fitness > 100) {
            CGP cgp(cgpConfig.numInputs, cgpConfig.numOutputs, cgpConfig.numRows, cgpConfig.numCols, cgpConfig.numNodeInputs);
            cgp.graph = graphs.at(bestGraphIndex).graph;
            return cgp;
        }
        //print(graphs.at(bestGraphIndex));
    }

    CGP cgp(cgpConfig.numInputs, cgpConfig.numOutputs, cgpConfig.numRows, cgpConfig.numCols, cgpConfig.numNodeInputs);
    cgp.graph = graphs.at(bestGraphIndex).graph;
    return cgp;
}