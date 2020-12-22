#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>

using namespace std;

minstd_rand randomEngine;

struct neuralnetwork {
    int p, q, r;
    vector<double> nIn, nHid, nOut;
    vector<vector<double>> inHid, hidOut;

    neuralnetwork(int p, int q, int r) : p(p), q(q), r(r),
        nIn(p + 1), nHid(q + 1), nOut(r),
        inHid(p + 1, vector<double>(q)), hidOut(q + 1, vector<double>(r)) {
        nIn[p] = nHid[q] = 1;
    }

    //aktivacijska funkcija za skriveni sloj
    double actfHid(double val) {
        return 2 * tanh(val) + 0.5;
    }

    //aktivacijska funkcija za izlazni sloj
    double actfOut(double val) {
        return 2 * tanh(val) + 0.5;
    }

    //raèunanje
    void propagate() {
        for (int i = 0; i < q; ++i)
            nHid[i] = 0;
        for (int i = 0; i <= p; ++i) for (int j = 0; j < q; ++j) {
            nHid[j] += nIn[i] * inHid[i][j];
        }
        for (int i = 0; i < q; ++i)
            nHid[i] = actfHid(nHid[i]);

        for (int i = 0; i < r; ++i)
            nOut[i] = 0;
        for (int i = 0; i <= q; ++i) for (int j = 0; j < r; ++j) {
            nOut[j] += nHid[i] * hidOut[i][j];
        }
        for (int i = 0; i < r; ++i)
            nOut[i] = actfOut(nOut[i]);
    }
};

constexpr double MUTATION_STRENGTH = 0.4;
constexpr double MUTATION_PROB = 0.2;

constexpr int POPULATION_SIZE = 100;
constexpr int P = 2, Q = 4, R = 1;
constexpr int INHID_GENES = (P + 1) * Q;
constexpr int HIDOUT_GENES = (Q + 1) * R;
constexpr int GENES_NUMBER = INHID_GENES + HIDOUT_GENES;
constexpr int TOURNAMENT_SIZE = 4;
constexpr int GENERATIONS = 1000000;

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

    //prebaci vrijednosti u težine mreže
    int k = 0;
    for (int i = 0; i <= P; ++i) for (int j = 0; j < Q; ++j) {
        nn.inHid[i][j] = individual[k++];
    }
    for (int i = 0; i <= Q; ++i) for (int j = 0; j < R; ++j) {
        nn.hidOut[i][j] = individual[k++];
    }

    //evaluiraj na XOR primjerima
    double loss = 0;
    for (int a = 0; a < 2; ++a) for (int b = 0; b < 2; ++b) {
        nn.nIn[0] = a;
        nn.nIn[1] = b;
        nn.propagate();

        double err = nn.nOut[0] - (a ^ b);
        loss += err * err;
    }

    return -loss;
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
        nn.nIn[0] = a;
        nn.nIn[1] = b;
        nn.propagate();

        cout << a << " " << b << " " << nn.nOut[0] << '\n';
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

//za crossover æe biti nasumièna linearna interpolacija izmeðu roditelja
void crossover(vector<double>& mom, vector<double>& dad, vector<double>& kid) {
    kid.clear();
    kid.reserve(GENES_NUMBER);
    static uniform_real_distribution<double> dist(0, 1);
    for (int i = 0; i < GENES_NUMBER; ++i) {
        kid.push_back(mom[i] + dist(randomEngine) * (dad[i] - mom[i]));
    }
    kid.shrink_to_fit();
}

//za mutaciju æe se nasumiène gene mrdnuti u stranu s normalnom distribucijom
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

int main() {
    randomEngine.seed(time(nullptr));

    fillPopulation();

    for (int gen = 1; gen <= GENERATIONS; ++gen) {
        shuffle(population.begin(), population.end(), randomEngine);

        calculateFitness();

        //ispis ponekad i za kraj
        int best = max_element(fitness.begin(), fitness.end()) - fitness.begin();
        if (gen % 2000 == 0 || -fitness[best] < 1e-5) {
            cout << -fitness[best] << '\n';
            print(population[best]);
            cout << flush;
            if (-fitness[best] < 1e-5) break;
        }

        //izbacujemo najlošijeg iz turnira i zamjenjujemo ga djetetom
        int worst = min_element(fitness.begin(), fitness.end()) - fitness.begin();
        swap(population[worst], population[2]);

        crossover(population[0], population[1], population[2]);
        mutate(population[2]);
    }
}