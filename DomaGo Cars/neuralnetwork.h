#pragma once
#include <vector>
#include <random>
#include "Jedinka.h"
#include "main.h"

struct neuralnetwork : Jedinka {
    int p, q, r;

    std::vector<double> nHid;
    std::vector<std::vector<double>> inHid, hidOut;

    neuralnetwork(int p, int q, int r) : Jedinka(p + 1, r), p(p), q(q), r(r),
        nHid(q + 1),
        inHid(p + 1, std::vector<double>(q)), hidOut(q + 1, std::vector<double>(r)) {
        inputs[p] = nHid[q] = 1;
    }

    //aktivacijska funkcija za skriveni sloj
    double actfHid(double val) {
        return 2 * tanh(val) + 0.5;
    }

    //aktivacijska funkcija za izlazni sloj
    double actfOut(double val) {
        return val;
    }

    //racunanje
    void propagate() {
        for (int i = 0; i < q; ++i)
            nHid[i] = 0;
        for (int i = 0; i <= p; ++i) for (int j = 0; j < q; ++j) {
            nHid[j] += inputs[i] * inHid[i][j];
        }
        for (int i = 0; i < q; ++i)
            nHid[i] = actfHid(nHid[i]);

        for (int i = 0; i < r; ++i)
            outputs[i] = 0;
        for (int i = 0; i <= q; ++i) for (int j = 0; j < r; ++j) {
            outputs[j] += nHid[i] * hidOut[i][j];
        }
        for (int i = 0; i < r; ++i)
            outputs[i] = actfOut(outputs[i]);
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

        int idx = max_element(outputs.begin(), outputs.end()) - outputs.begin();
        return idx + 1;

    }
};

vector<double> runNN();

void simulateNN(sf::RenderWindow& App, const vector<double>& individual);
void raceNN(sf::RenderWindow& App, const vector<double>& individual);