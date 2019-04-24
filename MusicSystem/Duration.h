#pragma once
#include "Errors.h"

class Duration {
	int numerator;
	int denominator;
public:

	Duration(int, int);
	Duration(int);
	//Duration();

	void changeDuration(int, int);
	int getDenominator() const;
	int getNumerator() const;

	static int getGCD(int, int);

	friend Duration operator+(const Duration&, const Duration&);
	friend ostream& operator<<(const ostream&, const Duration&);

	// Comparing durations
	friend bool operator<(const Duration&, const Duration&);
	friend bool operator>(const Duration&, const Duration&);
	friend bool operator==(const Duration&, const Duration&);
	friend bool operator<=(const Duration&, const Duration&);
	friend bool operator>=(const Duration&, const Duration&);
	
};