#pragma once
#include "Errors.h"

class Duration {
	int numerator;
	int denominator;
public:

	Duration(int, int);
	//Duration();

	void changeDuration(int, int);
	int getDenominator() const;
	int getNumerator() const;

	static int getGCD(int, int);

	friend Duration operator+(const Duration&, const Duration&);
};