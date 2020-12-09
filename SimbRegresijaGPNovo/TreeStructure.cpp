

#include <string>
#include <vector>
#include "TreeStructure.h"

using namespace std;


Input::Input() {
	numOfChildren = 0;
	name = "";
}

double Input::calculate(double input) {
	name = "X";
	return input;
}

Input* Input::copy() {
	Input* res = new Input();
	return res;
}

string Input::getName() {
	return name;
}


Constant::Constant() {
	numOfChildren = 0;
	value = rand() % 5 - 1;
	name = to_string(value);
}

Constant::Constant(double inputValue) {
	numOfChildren = 0;
	value = inputValue;
	name = to_string(inputValue);
}

double Constant::calculate(double input) {
	return value;
}

Constant* Constant::copy() {
	Constant* res = new Constant(value);
	return res;
}

string Constant::getName() {
	return name;
}

Add::Add() {
	numOfChildren = 2;
	name = "Add";
}

double Add::calculate(double input) {
	return children.at(0)->calculate(input) + children.at(1)->calculate(input);
}

Add* Add::copy() {
	Add* res = new Add();
	for (int i = 0; i < numOfChildren; i++) {
		res->children.push_back(children.at(i)->copy());
	}
	return res;
}

string Add::getName() {
	return name;
}

Sub::Sub() {
	numOfChildren = 2;
	name = "Sub";
}

double Sub::calculate(double input) {
	return children.at(0)->calculate(input) - children.at(1)->calculate(input);
}

Sub* Sub::copy() {
	Sub* res = new Sub();
	for (int i = 0; i < numOfChildren; i++) {
		res->children.push_back(children.at(i)->copy());
	}
	return res;
}

string Sub::getName() {
	return name;
}

Mul::Mul() {
	numOfChildren = 2;
	name = "Mul";
}

double Mul::calculate(double input) {
	return children.at(0)->calculate(input) * children.at(1)->calculate(input);
}

Mul* Mul::copy() {
	Mul* res = new Mul();
	for (int i = 0; i < numOfChildren; i++) {
		res->children.push_back(children.at(i)->copy());
	}
	return res;
}

string Mul::getName() {
	return name;
}

Div::Div() {
	numOfChildren = 2;
	name = "Div";
}

double Div::calculate(double input) {
	return children.at(0)->calculate(input) / children.at(1)->calculate(input);
}

Div* Div::copy() {
	Div* res = new Div();
	for (int i = 0; i < numOfChildren; i++) {
		res->children.push_back(children.at(i)->copy());
	}
	return res;
}

string Div::getName() {
	return name;
}