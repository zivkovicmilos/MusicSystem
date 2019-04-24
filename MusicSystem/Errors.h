#pragma once
#include <iostream>
using namespace std;

class InvalidDenominator : public exception {
public:
	string er = "Invalid denominator given\n";
	const char* what()const override {		return er.c_str();	}};

class InvalidOctave : public exception {
public:
	string er = "Invalid octave given\n";
	const char* what()const override {		return er.c_str();	}
};