#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
#include <time.h>
#include <math.h>

#include "CGP.h"
#include "main.h"

using namespace std;

const int TREE_DEPTH = 4;
const double MUTATION_PR = 0.4;
const int POPULATION_SIZE = 10;
const int GENERATIONS = 10;
//const int DIED = 800;

const int numFunctions = 4;

int numNodeInputs = 2;
int numInputs = 6;
int numOutputs = 2;
int numRows = 10;
int numCols = 10;

vector<Graph> graphs;

vector<int> randomGraph()
{
    vector<int> graph;

    for (int i = 0; i < numCols; i++)
    {
        int currMaxNodeOut = numInputs + i * numRows;
        for (int j = 0; j < numRows; j++)
        {
            graph.push_back(rand() % numFunctions);
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

double fitnessFunction(vector<int> graph)
{
    CGP cgp(numInputs, numOutputs, numRows, numCols, numNodeInputs);
    cgp.graph = graph;
    return evaluate(cgp);
}

void mutation(Graph& g)
{
    vector<int> graph = g.graph;
    int index = rand() % (graph.size());

    if (index < graph.size() - numOutputs)
    {
        if (index % (numNodeInputs + 1) == 0)
        {
            int functionId = rand() % numFunctions;
            graph.at(index) = functionId;
        }
        else
        {
            int indOfColumn = index / (numRows * (1 + numNodeInputs));
            int currMaxNodeOut = numInputs + indOfColumn * numRows;
            graph.at(index) = rand() % currMaxNodeOut;
        }
    }
    else
    {
        int outRandom = rand() % (numInputs + numCols * numRows);
        graph.at(index) = outRandom;
    }
    g.graph = graph;
    g.fitness = fitnessFunction(graph);
}

vector<int> crossover(vector<int>& mainGraph, vector<int>& otherGraph)
{
    vector<int> child = mainGraph;
    int index = rand() % (numRows * numCols);
    int startOfCut = index * (1 + numNodeInputs);
    for (startOfCut; startOfCut < mainGraph.size(); startOfCut++)
    {
        child.at(startOfCut) = otherGraph.at(startOfCut);
    }
    return child;
}

Graph crossAndReturnBestOfThree(Graph firstParent, Graph secondParent)
{
    Graph child(crossover(firstParent.graph, secondParent.graph), NULL);
    child.fitness = fitnessFunction(child.graph);

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

void fillPopulation()
{
    for (int i = 0; i < POPULATION_SIZE; i++)
    {
        vector<int> rG = randomGraph();
        cout << "pop " << i << endl;
        Graph randGraph = Graph(rG, fitnessFunction(rG));
        cout << "fitness pocetnog: " << fitnessFunction(rG) << endl;
        graphs.push_back(randGraph);
    }
}

double calculateFitness()
{
    double sumFitness = 0.;
    for (int j = 0; j < POPULATION_SIZE; j++)
    {
        double fitness = graphs.at(j).fitness;
        if (isnan(fitness))
            fitness = 0.;

        graphs.at(j).fitness = fitness;
        sumFitness += fitness;
    }
    cout << "izracunao sam fitness" << endl;
    return sumFitness;
}

vector<int> run()
{
    vector<Graph> graphs;
    for (int i = 0; i < POPULATION_SIZE; i++)
    {
        Graph randGraph = Graph(randomGraph(), 0);
        graphs.push_back(randGraph);
    }

    for (int i = 0; i < GENERATIONS; i++)
    {
        cout << i << endl;

        double sumFitness = 0.;
        for (int j = 0; j < POPULATION_SIZE; j++)
        {
            double fitness = fitnessFunction(graphs.at(j).graph);
            if (isnan(fitness))
                fitness = 0.;

            if (fitness > 1E+29)
                return graphs.at(j).graph;
            graphs.at(j).fitness = fitness;
            sumFitness += fitness;
        }

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

            double probability = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
            if (probability < MUTATION_PR)
            {
                mutation(offspring);
            }

            newGraphs.push_back(offspring);
        }
        graphs = newGraphs;
    }
    return graphs.at(POPULATION_SIZE - 1).graph;
}

Graph findBest()
{
    double maxFitness = graphs.at(0).fitness;
    int idxBest = 0;
    for (int i = 1; i < POPULATION_SIZE; i++) {
        if (graphs.at(i).fitness > maxFitness) {
            idxBest = i;
        }
    }
    return graphs.at(idxBest);
}

int main()
{
    init();

    fillPopulation();

    for (int gen = 1; gen <= GENERATIONS; ++gen) {

        cout << gen << endl;

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
            cout << "Napravio sam crossover" << endl;

            double probability = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
            if (probability < MUTATION_PR)
            {
                mutation(offspring);
                cout << "Napravio sam mutaciju" << endl;
            }

            newGraphs.push_back(offspring);
        }
        graphs = newGraphs;
    }


    Graph bestGraph = findBest();
    for (int i = 0; i < bestGraph.graph.size() - numOutputs; i++)
    {
        if (i % (numNodeInputs + 1) == 0)
            cout << endl;
        cout << bestGraph.graph.at(i);
    }
    cout << endl << "Output: ";
    for (int i = bestGraph.graph.size() - numOutputs; i < bestGraph.graph.size(); i++)
    {
        cout << bestGraph.graph.at(i) << " ";
    }

    cout << endl
        << "CGP Fitness: " << bestGraph.fitness << endl;

    return 0;

}