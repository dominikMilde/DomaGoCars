#pragma once
#include <vector>
#include <random>

struct neuralnetwork {
    int p, q, r;
    std::vector<double> nIn, nHid, nOut;
    std::vector<std::vector<double>> inHid, hidOut;

    neuralnetwork(int p, int q, int r) : p(p), q(q), r(r),
        nIn(p + 1), nHid(q + 1), nOut(r),
        inHid(p + 1, std::vector<double>(q)), hidOut(q + 1, std::vector<double>(r)) {
        nIn[p] = nHid[q] = 1;
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