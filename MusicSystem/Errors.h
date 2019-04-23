#pragma once
#include <iostream>
using namespace std;

class InvalidDenominator : public exception {
public:

	friend ostream operator<<(ostream& os, const InvalidDenominator& id) {
		return os << "Invalid denominator given\n";
	}
	
};

class InvalidOctave : public exception {
public:
	friend ostream operator<<(ostream& os, const InvalidOctave& io) {
		return os << "Invalid octave given\n";
	}
};