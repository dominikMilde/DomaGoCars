#ifndef BASIC_FTNS_H
#define BASIC_FTNS_H 1

#include <string>
#include <vector>

using namespace std;

class Node
{
public:
	string name;
	double fitness;
	int numOfChildren;
	vector<Node*> children;
	Node() { }
	virtual ~Node() { }
	virtual double calculate(double input) = 0;
	virtual Node* copy() = 0;
	virtual string getName() = 0;
};

class Input : public Node
{
public:
	Input();
	virtual double calculate(double input);
	Input* copy();
	virtual string getName();
};

class Constant : public Node
{
	double value;
public:
	Constant();
	Constant(double inputValue);
	virtual double calculate(double input);
	Constant* copy();
	virtual string getName();
};

class Add : public Node
{
public:
	Add();
	vector<Node*> children;
	virtual double calculate(double input);
	Add* copy();
	virtual string getName();
};

class Sub : public Node
{
public:
	Sub();
	vector<Node*> children;
	virtual double calculate(double input);
	Sub* copy();
	virtual string getName();
};

class Mul : public Node
{
public:
	Mul();
	vector<Node*> children;
	virtual double calculate(double input);
	Mul* copy();
	virtual string getName();
};


class Div : public Node
{
public:
	Div();
	vector<Node*> children;
	virtual double calculate(double input);
	Div* copy();
	virtual string getName();
};
#endif
