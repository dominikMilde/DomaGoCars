#pragma once 
#include <vector>
#include <random>
#include "Jedinka.h"

using namespace std;

const int SPEED_BREAK_POINT = 100;
const int ANGLE_BREAK_POINT = 100;


struct Graph {
    vector<int> graph;
    double fitness;
    Graph(vector<int> graph, double fitness) : graph(graph), fitness(fitness) {};
};

struct CGP : Jedinka {
    // num of inputs
    int n;
    // num of outputs
    int m;
    // num of rows
    int r;
    // num of columns
    int c;
    // num of node inputs
    int a;
    vector<int> graph;

    CGP(int n, int m, int r, int c, int a) : Jedinka(n, m), n(n), m(m), r(r), c(c), a(a), graph(c* r* (a + 1) + m) {}


    // racuna outpute za dani graf na racun danih inputa
    void propagate()
    {
        vector<double> nodeOutputs = inputs;
        //cout << "input: left and front " << inputs[5] << " " << inputs[1] << endl;
        for (int i = 0; i < c; i++)
        {
            vector<double> currNodeOutputs = nodeOutputs;
            for (int j = 0; j < r; j++)
            {
                int functionIdPos = (i * r + j) * (a + 1);
                int functionId = graph.at(functionIdPos);
                vector<int> nodeInputIds;
                for (int k = functionIdPos + 1; k < functionIdPos + 1 + a; k++)
                {
                    nodeInputIds.push_back(graph.at(k));
                }

                nodeOutputs.push_back(calculateFunction(currNodeOutputs, functionId, nodeInputIds));
            }
        }

        for (int i = 0; i < m; i++)
        {
            int pos = graph.at(graph.size() - m + i);
            outputs[i] = nodeOutputs.at(pos);
            //cout << "output: " << outputs[i] << endl;
        }
    }

    int akcija(vector<int> simulatorInputs) override
    {
        inputs[0] = simulatorInputs.at(0);
        inputs[1] = simulatorInputs.at(1);
        inputs[2] = simulatorInputs.at(2);
        inputs[3] = simulatorInputs.at(3);
        inputs[4] = simulatorInputs.at(4);
        inputs[5] = simulatorInputs.at(5);

        propagate();

        double speed = outputs.at(0);
        double angle = outputs.at(1);

        std::string action = "";

        if (speed < -SPEED_BREAK_POINT) action += "00"; //uspori
        else if (speed > SPEED_BREAK_POINT) action += "11"; //ubrzaj
        else action += "01"; //idle

        if (angle < -ANGLE_BREAK_POINT) action += "00"; //left
        else if (angle > ANGLE_BREAK_POINT) action += "11"; //right
        else action += "01"; //nista

        if (action == "0111") {
            return 1;
        } else if (action == "0100") {
            return 2;
        } else if (action == "0011") {
            return 3;
        } else if (action == "0000") {
            return 4;
        } else if (action == "1101") {
            return 5;
        } else if (action == "1100") {
            return 6;
        } else if (action == "1111") {
            return 7;
        }


        return -1;   
    }

    // racuna zadanu operaciju nad danim argumentima
    double calculateFunction(vector<double> calcOutputs, int functionId, vector<int> nodeInputIds)
    {

        vector<double> calcInputs;
        for (int i = 0; i < nodeInputIds.size(); i++)
        {
            calcInputs.push_back(calcOutputs.at(nodeInputIds.at(i)));
        }

        double rez = 0;

        switch (functionId)
        {
        case 0:
            for (int i = 0; i < calcInputs.size(); i++)
            {
                rez += calcInputs.at(i);
            }
            break;
        case 1:
            for (int i = 0; i < calcInputs.size(); i++)
            {
                if (i == 0)
                    rez = calcInputs.at(i);
                else
                    rez -= calcInputs.at(i);
            }
            break;
        case 2:
            for (int i = 0; i < calcInputs.size(); i++)
            {
                if (i == 0)
                    rez = 1;
                rez *= calcInputs.at(i);
            }
            break;
        case 3:
            for (int i = 0; i < calcInputs.size(); i++)
            {
                if (i == 0)
                    rez = calcInputs.at(i);
                else
                {
                    if (calcInputs.at(i) != 0)
                        rez /= calcInputs.at(i);
                    else
                        rez = 1E+37;
                }
            }
            break;
        }
        return rez;
    }
};

/*#ifndef DEF_CGP
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

#endif*/
