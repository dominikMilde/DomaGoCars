#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>
#include "main.h"
#include "neuralnetwork.h"

using namespace std;

minstd_rand randomEngine;

constexpr double MUTATION_STRENGTH = 0.4;
constexpr double MUTATION_PROB = 0.2;

constexpr int POPULATION_SIZE = 10;
constexpr int P = 6, Q = 6, R = 7;
constexpr int INHID_GENES = (P + 1) * Q;
constexpr int HIDOUT_GENES = (Q + 1) * R;
constexpr int GENES_NUMBER = INHID_GENES + HIDOUT_GENES;
constexpr int TOURNAMENT_SIZE = 3;
constexpr int GENERATIONS = 100000;

vector<vector<double>> population;
vector<double> fitness;

void fillPopulation() {
    population.clear();
    population.reserve(POPULATION_SIZE);

    //popuni populaciju sa svim vrijednostima random(0, 1)
    uniform_real_distribution<double> dist(-1, 1);
    for (int i = 0; i < POPULATION_SIZE; ++i) {
        vector<double> individual;

        for (int i = 0; i <= P; ++i) for (int j = 0; j < Q; ++j) {
            individual.push_back(dist(randomEngine));
        }
        for (int i = 0; i <= Q; ++i) for (int j = 0; j < R; ++j) {
            individual.push_back(dist(randomEngine));
        }

        population.push_back(individual);
    }

    population.shrink_to_fit();
}

double evaluate(const vector<double>& individual) {
    neuralnetwork nn(P, Q, R);

    //prebaci vrijednosti u tezine mreze
    int k = 0;
    for (int i = 0; i <= P; ++i) for (int j = 0; j < Q; ++j) {
        nn.inHid[i][j] = individual[k++];
    }
    for (int i = 0; i <= Q; ++i) for (int j = 0; j < R; ++j) {
        nn.hidOut[i][j] = individual[k++];
    }

    return evaluate(&nn);
}

void print(const vector<double>& individual) {
    neuralnetwork nn(P, Q, R);

    int k = 0;
    for (int i = 0; i <= P; ++i) for (int j = 0; j < Q; ++j) {
        nn.inHid[i][j] = individual[k++];
    }
    for (int i = 0; i <= Q; ++i) for (int j = 0; j < R; ++j) {
        nn.hidOut[i][j] = individual[k++];
    }

    for (int a = 0; a < 2; ++a) for (int b = 0; b < 2; ++b) {
        nn.inputs[0] = a;
        nn.inputs[1] = b;
        nn.propagate();

        cout << a << " " << b << " " << nn.outputs[0] << '\n';
    }
    /*for (int i = 0; i < GENES_NUMBER; ++i) {
        cout<<individual[i]<<" ";
    }
    cout<<endl;*/
}

void calculateFitness() {
    fitness.clear();
    fitness.reserve(TOURNAMENT_SIZE);

    for (int i = 0; i < TOURNAMENT_SIZE; ++i) {
        fitness.push_back(evaluate(population[i]));
    }

    fitness.shrink_to_fit();
}

//za crossover ce biti nasumicna linearna interpolacija izmeu roditelja
void crossover(vector<double>& mom, vector<double>& dad, vector<double>& kid) {
    kid.clear();
    kid.reserve(GENES_NUMBER);
    static uniform_real_distribution<double> dist(0, 1);
    for (int i = 0; i < GENES_NUMBER; ++i) {
        kid.push_back(mom[i] + dist(randomEngine) * (dad[i] - mom[i]));
    }
    kid.shrink_to_fit();
}

//za mutaciju ce se nasumicne gene mrdnuti u stranu s normalnom distribucijom
void mutate(vector<double>& individual) {
    //static uniform_int_distribution<int> posDist(0, GENES_NUMBER - 1);
    static normal_distribution<double> valDist(0, MUTATION_STRENGTH);
    static bernoulli_distribution choice(MUTATION_PROB);
    //int pos = posDist(randomEngine);
    for (int pos = 0; pos < GENES_NUMBER; ++pos) {
        if (choice(randomEngine)) {
            double val = valDist(randomEngine);
            individual[pos] += val;
        }
    }
}

void simulateNN(const vector<double>& individual) {
    neuralnetwork nn(P, Q, R);

    int k = 0;
    for (int i = 0; i <= P; ++i) for (int j = 0; j < Q; ++j) {
        nn.inHid[i][j] = individual[k++];
    }
    for (int i = 0; i <= Q; ++i) for (int j = 0; j < R; ++j) {
        nn.hidOut[i][j] = individual[k++];
    }

    simulate(&nn);
}


void runNN() {
    randomEngine.seed(time(nullptr));

    fillPopulation();

    for (int gen = 1; gen <= GENERATIONS; ++gen) {
        shuffle(population.begin(), population.end(), randomEngine);

        calculateFitness();

        //ispis ponekad i za kraj
        int best = max_element(fitness.begin(), fitness.end()) - fitness.begin();
        if (gen % 20 == 0 || -fitness[best] < 1e-5) {
            cout << "gen#" << gen << ": best fitness = " << fitness[best] << '\n';
            //print(population[best]);
            cout << flush;
        }

        //izbacujemo najlosijeg iz turnira i zamjenjujemo ga djetetom
        int worst = min_element(fitness.begin(), fitness.end()) - fitness.begin();
        swap(population[worst], population[2]);

        crossover(population[0], population[1], population[2]);
        mutate(population[2]);
        simulateNN(population[2]);
    }
}
