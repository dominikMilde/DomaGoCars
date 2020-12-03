#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>

using namespace std;

const double MUTATION_PR = 0.4;
const double CROSSING_PR = 0.6;

const int POPULATION_SIZE = 50;
const int GENES_NUMBER = 10;
const int TOURNAMENT_SIZE = 20;
const int GENERATIONS = 500;

vector<vector<int>> population;

void fillPopulation(){
    for(int ind=0; ind<POPULATION_SIZE; ind++){
        vector<int> individual;
        for(int gene=0; gene<GENES_NUMBER; gene++){
            individual.push_back((int) rand()%2);
        }
        population.push_back(individual);
    }
}

int indivudialFitness(vector<int> individual){
    int sum = 0;
    for(int gene=0; gene<GENES_NUMBER; gene++){
        sum += individual[gene];
    }
    return sum;
}

void mutate(vector<int> &individual){
    int positionToMutate =  rand() % GENES_NUMBER;
    if(individual[positionToMutate] == 0) 
        individual[positionToMutate] = 1;
    else
        individual[positionToMutate] = 1;
}

void crossing(int posFirstInd, int posSecondInd, vector<int> &nextGenIndividual){
    int crossingIndex = rand() % GENES_NUMBER;

    for(int gene = 0; gene <= crossingIndex; gene++){
        nextGenIndividual.push_back(population[posFirstInd][gene]); 
    }
    for(int gene = crossingIndex+1; gene < GENES_NUMBER; gene++){
        nextGenIndividual.push_back(population[posSecondInd][gene]); 
    }
}

int indexOfBestIndividual(){
    int maxValue = indivudialFitness(population[0]);
    int index = 0;
    for(int i=1; i<POPULATION_SIZE; i++){
        if(indivudialFitness(population[i])>maxValue){
            maxValue = indivudialFitness(population[i]);
            index = i;
        }
    }
    return index;
}

int main(){
    srand(time(NULL));
    fillPopulation();
    for(int generation=0; generation<GENERATIONS; generation++){
        for(int t=0; t<TOURNAMENT_SIZE; t++){
            double probability = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
            if(probability < CROSSING_PR){
                int indexOfFirst = rand() % POPULATION_SIZE;
                int indexOfSecond;
            
                do{
                    indexOfSecond = rand() % POPULATION_SIZE;
                }
                while (indexOfSecond == indexOfFirst);
                
                vector<int> nextGenInd;

                crossing(indexOfFirst, indexOfFirst, nextGenInd);
                probability = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
                if(probability<MUTATION_PR){
                    mutate(nextGenInd);
                }

                int fitnessOfFirst = indivudialFitness(population[indexOfFirst]);
                int fitnessOfSecond = indivudialFitness(population[indexOfSecond]);
                int fitnessSon = indivudialFitness(nextGenInd);

                int fitnessSmaller = fitnessOfFirst;
                int indexToChangeGenes = indexOfFirst;

                if(fitnessOfSecond < fitnessOfFirst){
                    fitnessSmaller = fitnessOfSecond;
                    indexToChangeGenes = indexOfSecond;
                }

                if(fitnessSon > fitnessSmaller){
                    for(int gene=0; gene<GENES_NUMBER; gene++){
                        population[indexToChangeGenes][gene] = nextGenInd[gene];
                    }
                }
            }
        }
        cout << "Generation: " << generation + 1 << endl;
        int maxScore = indivudialFitness(population[indexOfBestIndividual()]);
        cout << "Score of best individual: " <<  maxScore << endl;
        cout << "Genes of te best individual: ";
        for(int gene=0; gene<GENES_NUMBER; gene++){
            cout << population[indexOfBestIndividual()][gene];
        }
        cout << endl;

        cout << "------------------------------------" << endl;

        if(maxScore == GENES_NUMBER){
            break;
        }


    }
    system("pause");
    return 0;
}