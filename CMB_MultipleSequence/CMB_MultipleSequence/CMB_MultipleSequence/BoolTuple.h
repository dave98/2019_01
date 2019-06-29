#pragma once

#include <iostream>
using namespace std;

class BoolTuple
{
public:
	bool x;
	bool y;

	BoolTuple();
	BoolTuple(bool, bool);
	~BoolTuple();

	bool is_empty();
	void clear();
};



BoolTuple::BoolTuple()
{
	//Por defecto no apunta a ningun lugar en específico
	this->x = false; // = 0
	this->y = false; // = 0;

	//Izquierda (-1, 0) -> (true, false);
	//Diagonal (-1, -1) -> (true, true);
	//Arriba (0, -1) -> (false, true);
	//Vamos a abajo para arriba.
}


BoolTuple::BoolTuple(bool _x, bool _y) {
	this->x = _x;
	this->y = _y;
}

BoolTuple::~BoolTuple()
{
}

bool BoolTuple::is_empty() {
	return  !(bool)((int)this->x + (int)this->y);
}

void BoolTuple::clear() {
	this->x = false;
	this->y = false;
}

ostream& operator << (ostream& os, const BoolTuple& obj) {
	os << "(" << -1 * (int)obj.x << ";" << -1 * (int)obj.y << ")";
	return os;
}

class IntTuple {
public:
	int x;
	int y;
	IntTuple();
	IntTuple(int, int);
	~IntTuple();
};

IntTuple::IntTuple() {
	this->x = 0;
	this->y = 0;
}

IntTuple::IntTuple(int _x, int _y) {
	this->x = _x;
	this->y = _y;
}

IntTuple::~IntTuple() {

}

ostream& operator << (ostream& os, const IntTuple& obj) {
	os << "(" << obj.x << ";" << obj.y << ")";
	return os;
}