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


//neuron je klasa koja za svaki neuron cuva njegovu vrijednost, sve se racuna sa output value, ali ovaj initial sam ostavio da se moze pratiti i prosla vrijednost
//za input layer one su jednake, a ovaj inputNeuron je samo boolean varijabla koja oznacava je li cvor ulazni ili ne
class Neuron
{
private:
	float initialValue;
	float outputValue;
	bool inputNeuron;

	
//ovo dalje u klasi je sve samo inicijalizacija i getteri i setteri, ovaj output value na 1.0 je random vrijednost, to se sve izracuna kasnije kad su pravi brojevi
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


//klasa predstavlja neuronsku mrezu i ima vektore sa topologijom u koji idu 3 broja, 1. oznacava broj ulaznih neurona. 2. oznacava broj neurona u skrivenom sloju i
//3. oznacava broj izlaznih neurona
//u weightsInputHidden cuvaju se vrijednosti za weightse izmedu ulaznog i skrivenog sloja, znaci redom se vektor popunjava tako da za npr. topologiju 2 3 1
//vektor[0] ima vrijednost weighta izmedu prvog cvora u ulaznom sloju i prvog u skrivenom, vektor[1} za vezu drugog u ulaznom i prvog u skrivenom i tako dalje, prvi u ulazno, drugi u skrivenom,...
//weightsHiddenOutput isto to samo za skriveni i izlazni
//vektor neurons sprema sve neurone u mrezi redom za ulazni sloj pa za skriveni i onda za izlazni, broj kolko ima neurona u pojedinom sloju gledam po topologiji, znaci
//topology[0] je broj u ulaznom, [1] u skrivenom i [2] u izlaznom
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
		
		//ovdje se u konstruktoru inicijaliziraju vrijednosti za weightsInputHidden, stavio sam da moze biti najvise 5, to mozemo promijeniti kad vidimo kako ce raditi mreza
		//kad izbaci random float broj izmedu 0 i 5 onda opet slucajnim odabirom izabire predznak - ili + sa jednakom sansom i vrijednost se doda u vektor
		for (int i = 0; i < topology[0] * topology[1]; i++) {
			float x = 5.0;
			float randomWeight = float(rand()) / float((RAND_MAX)) * x;
			if (rand() / float(RAND_MAX) < 0.5) {
				randomWeight = -randomWeight;
			}
			weightsInputHidden.push_back(randomWeight);
		}
		
		//isto kao i gore samo za weightsHiddenOutput
		for (int i = 0; i < topology[1] * topology[2]; i++) {
			float x = 5.0;
			float randomWeight = float(rand()) / float((RAND_MAX)) * x;
			if (rand() / float(RAND_MAX) < 0.5) {
				randomWeight = -randomWeight;
			}
			weightsHiddenOutput.push_back(randomWeight);
		}
		
		//ovdje se u vektor neurona dodaju ulazni neuroni i njima se vrijednost stavi na onu koju smo npr. ucitali iz datoteke, znaci cisit ulazni podaci
		for (Neuron n : inputNeurons) {
			neurons.push_back(n);
			cout << n.getInitialValue() << endl;
		}
		
		//ovdje se dodaju preostali neuroni, znaci treba ih dodati jos za skriveni i izlazni sloj pa se zbroje vrijednost topology[1] i [2] i toliko ih se doda
		//vrijednost im je random stavljena na 1.0, ali kasnije se izracuna prava vrijednost, ova je skroz nebitna
		for (int i = 0; i < topology[1] + topology[2]; i++) {
			Neuron neuron = Neuron(1.0, false);
			neurons.push_back(neuron);
			cout << neuron.getInitialValue() << endl;
		}
	}

	//getteri
	vector<unsigned> getTopology() { return topology; }
	vector<float> getWeightsInputHidden() { return weightsInputHidden; }
	vector<float> getWeightsHiddenOutput() { return weightsHiddenOutput; }
	vector<Neuron> getNeurons() { return neurons; }

	//ovo je funkcija koja na osnovu output vrijednosti proslog sloja i vrijednosti weighta koji je izmedu neurona u proslom i u sadasnjem sloju
	void calculateOutputValues() {
		int currentHidden = 0;
		int currentInput = 0;
		int currentOutput = 0;
		int inputLayerNeurons = (int)(topology[0]);
		int hiddenLayerNeurons = (int)(topology[1]);
		int outputLayerNeurons = (int)(topology[2]);
		
		//loop da se prode kroz sve neurone u mrezi
		for (Neuron n : neurons) {
			//ako je inputNeuron true onda je to ulazni sloj i njegova output vrijednost, odnosno vrijednost s koja se salje dalje i s kojom se racuna
			//jednaka onoj vrijednosti s kojom smo ga inicijalizirali
			if (n.getInputNeuron()) {
				neurons[currentInput].setOutputValue(n.getInitialValue());
				currentInput += 1;
			}
			//ako nije ulazni sloj
			else {
				//current Hidden samo broji koliko se preslo dosada skrivenih neurona i kad se produ svi prebacuje se na izlazni sloj
				//za svaki neuron u skrivenom sloju u ov se zbrajaju output vrijednost svakog cvora u ulaznom i vrijednost weighta(pisem weight cijelo vrijeme jer nisam siguran
				//koji je prijevod pa da nema zabune), znaci npr. topologija 2 3 1, ov za prvi neuron u skrivenom sloju ce biti:
				//outputValue(1. neuron ulaznog sloja)*weight(izmedu 1. u ulaznom i 1. u skrivenom) + outputValue(2. neuron ulaznog sloja)*weight(izmedu 2. u ulaznom i 1. u skrivenom)
				//kad se dobije ov za taj neuron onda se on ubaci u formulu za sigmoidu, to sam stavio za funkciju aktivacije, znaci za bilo koju vrijednost ov-a 
				//sigmoida izbaci vrijednost izmedu 0 i 1 po formuli 1/1+e**-ov i onda se ta dobivena vrijednost stavi za outputValue tog neurona
				float ov = 0.0;
				if (currentHidden < hiddenLayerNeurons) {
					int j = 0;
					//ovaj for loop se moze mozda i jednostavnije, ali u principu imamo npr. ako smo na 1. neuronu skrivenog sloja sa topologijom 2 3 1 onda je njegov indeks u
					//vektoru sa neuronima 2 jer su neurons[0] i neurons[1] neuroni iz ulaznog
					for (int i = currentHidden * inputLayerNeurons; i < currentHidden * inputLayerNeurons + inputLayerNeurons; i++) {
						ov += weightsInputHidden[i] * neurons[j].getOutputValue();
						j += 1;
					}
					neurons[currentHidden + inputLayerNeurons].setOutputValue(1/(1 + exp(-ov)));
					currentHidden += 1;
				
				//kad se produ svi neuroni u skrivenom onda se racunaju outValue-i za izlazni sloj po istom principu kao i iznad samo za skriveni i izlazni
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
	//topologija
	vector<unsigned> a = { 2,4,1};

	//ulazni neuroni sa vrijednosti 2.0 i 3.0
	Neuron n1 = Neuron(3.0, true);
	Neuron n2 = Neuron(2.0, true);
	vector<Neuron> neurons = { n1, n2 };

	//incijalizacija neuronske mreze
	NeuralNetwork nn = NeuralNetwork(a, neurons);

	vector<unsigned> top = nn.getTopology();
	vector<float> w1 = nn.getWeightsInputHidden();
	vector<float> w2 = nn.getWeightsHiddenOutput();

	//ispis topologije
	cout << endl;
	for (int n : top) {
		cout << n << '\n';
	}
	
	//ispis weightova izmedu ulaznog i skrivenog
	cout << endl;
	for (float n : w1) {
		cout << n << '\n';
	}
	
	//ispis weightova izmedu skrivenog i izlaznog
	cout << endl;
	for (float n : w2) {
		cout << n << '\n';
	}

	nn.calculateOutputValues();
	cout << "---------------------------------" << endl;

	//ispis outputValuea za svaki neuron
	for (Neuron n : nn.getNeurons()) {
		cout << n.getOutputValue() << endl;
	}

}