#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>
#include "main.h"
#include "neuralnetwork.h"
#include "config.h"

using namespace std;

minstd_rand randomEngine;

vector<vector<double>> population;
vector<double> fitness;

void fillPopulation() {
    population.clear();
    population.reserve(neuralNetworkConfig.populationSize);

    cout << "Generating initial population..." << endl;

    //popuni populaciju sa svim vrijednostima random(0, 1)
    uniform_real_distribution<double> dist(-1, 1);
    for (int i = 0; i < neuralNetworkConfig.populationSize; ++i) {
        vector<double> individual;

        for (int i = 0; i <= neuralNetworkConfig.p; ++i) for (int j = 0; j < neuralNetworkConfig.q; ++j) {
            individual.push_back(dist(randomEngine));
        }
        for (int i = 0; i <= neuralNetworkConfig.q; ++i) for (int j = 0; j < neuralNetworkConfig.r; ++j) {
            individual.push_back(dist(randomEngine));
        }

        population.push_back(individual);
    }

    population.shrink_to_fit();
}

double evaluate(const vector<double>& individual) {
    neuralnetwork nn(neuralNetworkConfig.p, neuralNetworkConfig.q, neuralNetworkConfig.r);

    //prebaci vrijednosti u tezine mreze
    int k = 0;
    for (int i = 0; i <= neuralNetworkConfig.p; ++i) for (int j = 0; j < neuralNetworkConfig.q; ++j) {
        nn.inHid[i][j] = individual[k++];
    }
    for (int i = 0; i <= neuralNetworkConfig.q; ++i) for (int j = 0; j < neuralNetworkConfig.r; ++j) {
        nn.hidOut[i][j] = individual[k++];
    }

    return evaluate(&nn);
}

void print(const vector<double>& individual) {
    neuralnetwork nn(neuralNetworkConfig.p, neuralNetworkConfig.q, neuralNetworkConfig.r);

    int k = 0;
    for (int i = 0; i <= neuralNetworkConfig.p; ++i) for (int j = 0; j < neuralNetworkConfig.q; ++j) {
        nn.inHid[i][j] = individual[k++];
    }
    for (int i = 0; i <= neuralNetworkConfig.q; ++i) for (int j = 0; j < neuralNetworkConfig.r; ++j) {
        nn.hidOut[i][j] = individual[k++];
    }

    for (int a = 0; a < 2; ++a) for (int b = 0; b < 2; ++b) {
        nn.inputs[0] = a;
        nn.inputs[1] = b;
        nn.propagate();

        cout << a << " " << b << " " << nn.outputs[0] << '\n';
    }
    /*for (int i = 0; i < neuralNetworkConfig.genesNumber; ++i) {
        cout<<individual[i]<<" ";
    }
    cout<<endl;*/
}

int findBest() {
    double best = evaluate(population[0]);
    int bestIdx = 0;

    for (int i = 1; i < neuralNetworkConfig.populationSize; ++i) {
        double tmp = evaluate(population[i]);
        if (tmp > best) {
            best = tmp;
            bestIdx = i;
        }
    }

    return bestIdx;
}

void calculateFitness() {
    fitness.clear();
    fitness.reserve(neuralNetworkConfig.tournamentSize);

    for (int i = 0; i < neuralNetworkConfig.tournamentSize; ++i) {
        fitness.push_back(evaluate(population[i]));
    }

    fitness.shrink_to_fit();
}

//za crossover ce biti nasumicna linearna interpolacija izmeu roditelja
void crossover(vector<double>& mom, vector<double>& dad, vector<double>& kid) {
    kid.clear();
    kid.reserve(neuralNetworkConfig.genesNumber);
    static uniform_real_distribution<double> dist(0, 1);
    for (int i = 0; i < neuralNetworkConfig.genesNumber; ++i) {
        kid.push_back(mom[i] + dist(randomEngine) * (dad[i] - mom[i]));
    }
    kid.shrink_to_fit();
}

//za mutaciju ce se nasumicne gene mrdnuti u stranu s normalnom distribucijom
void mutate(vector<double>& individual) {
    //static uniform_int_distribution<int> posDist(0, neuralNetworkConfig.genesNumber - 1);
    static normal_distribution<double> valDist(0, neuralNetworkConfig.mutationStrength);
    static bernoulli_distribution choice(neuralNetworkConfig.mutationProb);
    //int pos = posDist(randomEngine);
    for (int pos = 0; pos < neuralNetworkConfig.genesNumber; ++pos) {
        if (choice(randomEngine)) {
            double val = valDist(randomEngine);
            individual[pos] += val;
        }
    }
}

void simulateNN(sf::RenderWindow& App, const vector<double>& individual) {
    neuralnetwork nn(neuralNetworkConfig.p, neuralNetworkConfig.q, neuralNetworkConfig.r);

    int k = 0;
    for (int i = 0; i <= neuralNetworkConfig.p; ++i) for (int j = 0; j < neuralNetworkConfig.q; ++j) {
        nn.inHid[i][j] = individual[k++];
    }
    for (int i = 0; i <= neuralNetworkConfig.q; ++i) for (int j = 0; j < neuralNetworkConfig.r; ++j) {
        nn.hidOut[i][j] = individual[k++];
    }

    simulate(App, &nn);
}

void raceNN(sf::RenderWindow& App, const vector<double>& individual) {
    neuralnetwork nn(neuralNetworkConfig.p, neuralNetworkConfig.q, neuralNetworkConfig.r);

    int k = 0;
    for (int i = 0; i <= neuralNetworkConfig.p; ++i) for (int j = 0; j < neuralNetworkConfig.q; ++j) {
        nn.inHid[i][j] = individual[k++];
    }
    for (int i = 0; i <= neuralNetworkConfig.q; ++i) for (int j = 0; j < neuralNetworkConfig.r; ++j) {
        nn.hidOut[i][j] = individual[k++];
    }

    simulateRace(App, &nn);
}

vector<double> runNN() {
    cout << "Started neural network learning" << endl
        << "Number of generations: " << neuralNetworkConfig.generations << endl
        << "Population size: " << neuralNetworkConfig.populationSize << endl << endl;

    randomEngine.seed(time(nullptr));

    fillPopulation();

    for (int gen = 1; gen <= neuralNetworkConfig.generations; ++gen) {
        shuffle(population.begin(), population.end(), randomEngine);

        calculateFitness();

        //ispis ponekad i za kraj
        int best = max_element(fitness.begin(), fitness.end()) - fitness.begin();
        if (gen % 20 == 0 || -fitness[best] < 1e-5) {
            cout << endl << endl << "gen#" << gen << ": best fitness = " << fitness[best] << '\n';
            cout << flush;
        }

        //izbacujemo najlosijeg iz turnira i zamjenjujemo ga djetetom
        int worst = min_element(fitness.begin(), fitness.end()) - fitness.begin();
        swap(population[worst], population[2]);

        crossover(population[0], population[1], population[2]);
        mutate(population[2]);
        //simulateNN(population[best]);
    }

    cout << endl << endl;

    int best = findBest();
    return population[best];
}
