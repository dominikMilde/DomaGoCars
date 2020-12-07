#include <vector>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <fstream>
#include <sstream>
#include <time.h>
#include <math.h>

using namespace std;

class Neuron
{
private:
	float initialValue;
	float outputValue;
	bool inputNeuron;

public:
	Neuron(float initialValue1, bool inputNeuron1) {
		initialValue = initialValue1;
		inputNeuron = inputNeuron1;
		if (inputNeuron) {
			outputValue = initialValue;
		}
		else {
			outputValue = 1.0;
		}
	}

	float getInitialValue() { return initialValue; }
	float getOutputValue() { return outputValue; }
	bool getInputNeuron() { return inputNeuron; }

	void setOutputValue(float outputValue) {
		this->outputValue = outputValue;
	}
	void setInitialValue(float initialValue) {
		this->initialValue = initialValue;
	}

};


class NeuralNetwork
{
private:
	vector<unsigned> topology;
	vector<float> weightsInputHidden;
	vector<float> weightsHiddenOutput;
	vector<Neuron> neurons;

public:
	NeuralNetwork(vector<unsigned> topology1, vector<Neuron> inputNeurons) {
		topology = topology1;
		srand((unsigned int)time(NULL));
		for (int i = 0; i < topology[0] * topology[1]; i++) {
			float x = 5.0;
			float randomWeight = float(rand()) / float((RAND_MAX)) * x;
			if (rand() / float(RAND_MAX) < 0.5) {
				randomWeight = -randomWeight;
			}
			weightsInputHidden.push_back(randomWeight);
		}
		for (int i = 0; i < topology[1] * topology[2]; i++) {
			float x = 5.0;
			float randomWeight = float(rand()) / float((RAND_MAX)) * x;
			if (rand() / float(RAND_MAX) < 0.5) {
				randomWeight = -randomWeight;
			}
			weightsHiddenOutput.push_back(randomWeight);
		}
		for (Neuron n : inputNeurons) {
			neurons.push_back(n);
			cout << n.getInitialValue() << endl;
		}
		for (int i = 0; i < topology[1] + topology[2]; i++) {
			Neuron neuron = Neuron(1.0, false);
			neurons.push_back(neuron);
			cout << neuron.getInitialValue() << endl;
		}
	}

	vector<unsigned> getTopology() { return topology; }
	vector<float> getWeightsInputHidden() { return weightsInputHidden; }
	vector<float> getWeightsHiddenOutput() { return weightsHiddenOutput; }
	vector<Neuron> getNeurons() { return neurons; }

	void calculateOutputValues() {
		int currentHidden = 0;
		int currentInput = 0;
		int currentOutput = 0;
		int inputLayerNeurons = (int)(topology[0]);
		int hiddenLayerNeurons = (int)(topology[1]);
		int outputLayerNeurons = (int)(topology[2]);
		for (Neuron n : neurons) {
			if (n.getInputNeuron()) {
				neurons[currentInput].setOutputValue(n.getInitialValue());
				currentInput += 1;
			}
			else {
				float ov = 0.0;
				if (currentHidden < hiddenLayerNeurons) {
					int j = 0;
					for (int i = currentHidden * inputLayerNeurons; i < currentHidden * inputLayerNeurons + inputLayerNeurons; i++) {
						ov += weightsInputHidden[i] * neurons[j].getOutputValue();
						j += 1;
					}
					neurons[currentHidden + inputLayerNeurons].setOutputValue(1/(1 + exp(-ov)));
					currentHidden += 1;
				} else if (currentOutput < outputLayerNeurons) {
					int j = 0;
					for (int i = currentOutput * hiddenLayerNeurons ; i < currentOutput * hiddenLayerNeurons + hiddenLayerNeurons; i++) {
						ov += weightsHiddenOutput[i] * neurons[j+inputLayerNeurons].getOutputValue();
						j += 1;
					}
					neurons[currentOutput + inputLayerNeurons + hiddenLayerNeurons].setOutputValue(1 / (1 + exp(-ov)));
					currentOutput += 1;
				}
			}
		}
	}
};

int main() {
	vector<unsigned> a = { 2,4,1};

	Neuron n1 = Neuron(3.0, true);
	Neuron n2 = Neuron(2.0, true);
	vector<Neuron> neurons = { n1, n2 };

	NeuralNetwork nn = NeuralNetwork(a, neurons);

	vector<unsigned> top = nn.getTopology();
	vector<float> w1 = nn.getWeightsInputHidden();
	vector<float> w2 = nn.getWeightsHiddenOutput();

	cout << endl;
	for (int n : top) {
		cout << n << '\n';
	}
	cout << endl;
	for (float n : w1) {
		cout << n << '\n';
	}
	cout << endl;
	for (float n : w2) {
		cout << n << '\n';
	}

	nn.calculateOutputValues();
	cout << "---------------------------------" << endl;

	for (Neuron n : nn.getNeurons()) {
		cout << n.getOutputValue() << endl;
	}

}