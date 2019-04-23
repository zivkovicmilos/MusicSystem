#pragma once
#include "Errors.h"

class Duration {
	int numerator;
	int denominator;

private:

	Duration(int, int);

	void changeDuration(int, int);
	int getDenominator() const;
	int getNumerator() const;

	static int getGCD(int, int);

	friend Duration operator+(const Duration&, const Duration&);
};