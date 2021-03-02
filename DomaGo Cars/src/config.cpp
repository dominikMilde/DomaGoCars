#include <iostream>
#include "libconfig.h++"

#include "config.h"

using namespace std;
using namespace libconfig;

DgcConfig globalConfig;
NeuralNetworkConfig neuralNetworkConfig;
CgpConfig cgpConfig;

void initConfig() {
	Config dgcConfig;
	dgcConfig.readFile("data/dgc.cfg");
	Setting& mainSetting = dgcConfig.lookup("main");
	Setting& simulatorSetting = dgcConfig.lookup("simulator");
	Setting& neuralNetworkSetting = dgcConfig.lookup("neuralNetwork");
	Setting& cgpSetting = dgcConfig.lookup("cgp");
	
	dgcConfig.lookupValue("akcija", globalConfig.akcija);

	mainSetting.lookupValue("fitnessKoef", globalConfig.fitnessKoef);
	mainSetting.lookupValue("numOfLaps", globalConfig.numOfLaps);
	mainSetting.lookupValue("maxSimTime", globalConfig.maxSimTime);
	mainSetting.lookupValue("maxSimDist", globalConfig.maxSimDist);
	mainSetting.lookupValue("maxEvaTime", globalConfig.maxEvaTime);
	mainSetting.lookupValue("maxEvaDist", globalConfig.maxEvaDist);

	Setting& tracksArray = mainSetting.lookup("tracks");
	globalConfig.tracks.clear();
	for (Setting& trackSetting : tracksArray) {
		globalConfig.tracks.push_back(trackSetting);
	}
	mainSetting.lookupValue("displayedTrack", globalConfig.displayedTrack);

	simulatorSetting.lookupValue("gasAcc", globalConfig.gasAcc);
	simulatorSetting.lookupValue("brakeAcc", globalConfig.brakeAcc);
	simulatorSetting.lookupValue("rr", globalConfig.rr);
	simulatorSetting.lookupValue("drag", globalConfig.drag);
	simulatorSetting.lookupValue("steeringRate", globalConfig.steeringRate);
	simulatorSetting.lookupValue("steeringReturnRate", globalConfig.steeringReturnRate);
	simulatorSetting.lookupValue("maxSteeringAngle", globalConfig.maxSteeringAngle);
	simulatorSetting.lookupValue("rotationIdx0", globalConfig.rotationIdx0);
	simulatorSetting.lookupValue("rotationIdx1", globalConfig.rotationIdx1);
	simulatorSetting.lookupValue("blok", globalConfig.blok);

	neuralNetworkSetting.lookupValue("mutationStrength", neuralNetworkConfig.mutationStrength);
	neuralNetworkSetting.lookupValue("mutationProb", neuralNetworkConfig.mutationProb);
	
	neuralNetworkSetting.lookupValue("populationSize", neuralNetworkConfig.populationSize);
	neuralNetworkSetting.lookupValue("p", neuralNetworkConfig.p);
	neuralNetworkSetting.lookupValue("q", neuralNetworkConfig.q);
	neuralNetworkSetting.lookupValue("r", neuralNetworkConfig.r);
	neuralNetworkConfig.inHidGenes = (neuralNetworkConfig.p + 1) * neuralNetworkConfig.q;
	neuralNetworkConfig.hidOutGenes = (neuralNetworkConfig.q + 1) * neuralNetworkConfig.r;
	neuralNetworkConfig.genesNumber = neuralNetworkConfig.inHidGenes + neuralNetworkConfig.hidOutGenes;
	neuralNetworkSetting.lookupValue("tournamentSize", neuralNetworkConfig.tournamentSize);
	neuralNetworkSetting.lookupValue("generations", neuralNetworkConfig.generations);

	cgpSetting.lookupValue("breakpoint", cgpConfig.breakpoint);

	cgpSetting.lookupValue("mutationProb", cgpConfig.mutationProb);
	cgpSetting.lookupValue("mutationRate", cgpConfig.mutationRate);
	cgpSetting.lookupValue("populationSize", cgpConfig.populationSize);
	cgpSetting.lookupValue("generations", cgpConfig.generations);
	cgpSetting.lookupValue("numFunctions", cgpConfig.numFunctions);

	cgpSetting.lookupValue("numNodeInputs", cgpConfig.numNodeInputs);
	cgpSetting.lookupValue("numInputs", cgpConfig.numInputs);
	cgpSetting.lookupValue("numOutputs", cgpConfig.numOutputs);
	cgpSetting.lookupValue("numRows", cgpConfig.numRows);
	cgpSetting.lookupValue("numCols", cgpConfig.numCols);
	cgpConfig.graphSize = cgpConfig.numCols * cgpConfig.numRows * (cgpConfig.numNodeInputs + 1) + cgpConfig.numOutputs;
}