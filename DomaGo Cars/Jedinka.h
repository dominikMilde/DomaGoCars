#pragma once
#include <vector>

using namespace std;

struct Jedinka {
    virtual int akcija(vector<int> simulatorInputs) = 0;
    //virtual void runJedinka() = 0;
    vector<double> inputs;
    vector<double> outputs;
    Jedinka(int n, int m) : inputs(n), outputs(m) {};
};