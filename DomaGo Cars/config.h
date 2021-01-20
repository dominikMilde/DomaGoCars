#pragma once

struct DgcConfig {
	//start
	int nodeInputs;
	int inputs;
	int outputs;
	int rows;
	int cols;

	//main
	int fitnessKoef;
	int maxSimTime;
	int maxSimDist;
	int maxEvaTime;
	int maxEvaDist;

	//simulator
	double gasAcc;
	double brakeAcc;
	double rr;
	double drag;
	double steeringRate;
	double steeringReturnRate;
	double maxSteeringAngle;
	double rotationIdx0;
	double rotationIdx1;
	int blok;
} extern globalConfig;

struct NeuralNetworkConfig {
	double mutationStrength;
	double mutationProb;

	int populationSize;
	int p, q, r;
	int inHidGenes;
	int hidOutGenes;
	int genesNumber;
	int tournamentSize;
	int generations;
} extern neuralNetworkConfig;

struct CgpConfig {
	int breakpoint;

	double mutationProb;
	double mutationRate;
	int populationSize;
	int generations;
	int numFunctions;

	int numNodeInputs;
	int numInputs;
	int numOutputs;
	int numRows;
	int numCols;
	int graphSize;
} extern cgpConfig;


void initConfig();