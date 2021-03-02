#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "storage.h"

using namespace std;

void storeCgpDriver(vector<int> driver) {
	ofstream driverFile;
	driverFile.open("data/cgpDriver.txt", std::ofstream::out | std::ofstream::trunc);
	for (int i = 0; i < driver.size(); i++) {
		driverFile << driver.at(i) << " ";
	}
	driverFile.close();
}

vector<int> readCgpDriver() {
	string text;
	ifstream driverFile("data/cgpDriver.txt");
	while (getline(driverFile, text)) {}
	driverFile.close();

	vector<int> driver;
	stringstream ss(text);
	int number;
	while (ss >> number) {
		driver.push_back(number);
	}
	return driver;
}

void storeNnDriver(vector<double> driver) {
	ofstream driverFile;
	driverFile.open("data/nnDriver.txt", std::ofstream::out | std::ofstream::trunc);
	for (int i = 0; i < driver.size(); i++) {
		driverFile << driver.at(i) << " ";
	}
	driverFile.close();
}

vector<double> readNnDriver() {
	string text;
	ifstream driverFile("data/nnDriver.txt");
	while (getline(driverFile, text)) {}
	driverFile.close();

	vector<double> driver;
	istringstream s(text);
	double d;
	while (s >> d) {
		driver.push_back(d);
	}
	return driver;
}
