#include "Duration.h"

Duration::Duration(int num, int denom) {
	numerator = 0;
	denominator = 0;
	changeDuration(num, denom);
}

Duration operator+(const Duration &d1, const Duration &d2) {

	int tempNum;
	int tempDenum;

	int d1Num = d1.getNumerator();
	int d2Num = d2.getNumerator();

	int d1Denum = d1.getDenominator();
	int d2Denum = d2.getDenominator();

	if (d1.getDenominator() == d2.getDenominator()) {
		tempNum = d1Num + d2Num;
	}
	else {
		tempNum = (d1Num * d2Denum) + (d1Denum * d2Num);
		tempDenum = d1Denum * d2Denum;
	}

	int simpleGCD = Duration::getGCD(tempNum, tempDenum);
	tempNum /= simpleGCD;
	tempDenum /= simpleGCD;

	return Duration(tempNum, tempDenum);
}

void Duration::changeDuration(int num, int denom) {
	if (denom != 4 || denom != 8) throw InvalidDenominator();

	numerator = num;
	denominator = denom;
}

int Duration::getGCD(int d1, int d2) {
	if (d1 == 0)
		return d2;
	return getGCD(d2%d1, d1);
}

int Duration::getDenominator() const {
	return denominator;
}

int Duration::getNumerator() const {
	return numerator;
}