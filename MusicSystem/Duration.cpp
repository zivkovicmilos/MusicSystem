#include "Duration.h"

Duration::Duration(int num, int denom) {
	if (((denom != 4)) && (denom != 8)) throw InvalidDenominator();
	numerator = 0;
	denominator = 0;
	changeDuration(num, denom);
}
Duration::Duration(int num = 0) {
	numerator = denominator = 0;
}

Duration operator+(const Duration &d1, const Duration &d2) {

	// Check to see if one duration is 0
	if (d1.getDenominator() == 0) {
		return d2;
	}
	else if (d2.getDenominator() == 0) {
		return d1;
	}

	int tempNum;
	int tempDenom;

	int d1Num = d1.getNumerator();
	int d2Num = d2.getNumerator();

	int d1Denom = d1.getDenominator();
	int d2Denom = d2.getDenominator();

	if (d1.getDenominator() == d2.getDenominator()) {
		tempNum = d1Num + d2Num;
		tempDenom = d1.getDenominator();
	}
	else {
		tempNum = (d1Num * d2Denom) + (d1Denom * d2Num);
		tempDenom = d1Denom * d2Denom;
	}
	
	if ((tempDenom != 4) && (tempDenom != 8)) {
		if (tempDenom == tempNum) {
			tempDenom /= 4;
			tempNum /= 4;
		
		}
		else {
			int simpleGCD = Duration::getGCD(tempNum, tempDenom);
			tempNum /= simpleGCD;
			tempDenom /= simpleGCD;
		}
	}
	
	if (tempDenom == 2) {
		tempNum *= 2;
		tempDenom *= 2;
	}
	
	return Duration(tempNum, tempDenom);
}

void Duration::changeDuration(int num, int denom) {
	numerator = num;
	denominator = denom;
}

// Fix so that the denominators are always either 4 or 8
int Duration::getGCD(int d1, int d2) {
	if (d1 == 0) {
		return d2;
	}
	return getGCD(d2%d1, d1);
}

int Duration::getDenominator() const {
	return denominator;
}

int Duration::getNumerator() const {
	return numerator;
}

bool operator==(const Duration& d1, const Duration& d2) {
	
	int d1Num = d1.getNumerator();
	int d2Num = d2.getNumerator();

	int d1Denom = d1.getDenominator();
	int d2Denom = d2.getDenominator();

	if (((double)d1Num/d1Denom) == ((double)d2Num/d2Denom)) {
		return true;
	}
	else {
		return false;
	}
}

bool operator<(const Duration& d1, const Duration& d2) {
	int d1Num = d1.getNumerator();
	int d2Num = d2.getNumerator();

	int d1Denom = d1.getDenominator();
	int d2Denom = d2.getDenominator();

	if (((double)d1Num / d1Denom) < ((double)d2Num / d2Denom)) {
		return true;
	}
	else {
		return false;
	}
}

bool operator>(const Duration& d1, const Duration& d2) {
	int d1Num = d1.getNumerator();
	int d2Num = d2.getNumerator();

	int d1Denom = d1.getDenominator();
	int d2Denom = d2.getDenominator();

	if (((double)d1Num / d1Denom) > ((double)d2Num / d2Denom)) {
		return true;
	}
	else {
		return false;
	}
}

bool operator<=(const Duration& d1, const Duration& d2) {
	if ((d1 == d2) || (d1 < d2)) {
		return true;
	}
	else {
		return false;
	}
}

bool operator>=(const Duration& d1, const Duration& d2) {
	if ((d1 == d2) || (d1 > d2)) {
		return true;
	}
	else {
		return false;
	}
}

ostream& operator<<(ostream& os, Duration& d) {
	int num = d.getNumerator();
	int denom = d.getDenominator();
	return os << num << "/" << denom;
}