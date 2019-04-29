#include "Measure.h"

Measure::Measure(Duration d) : maxDur(d), currDur(0) {}

vector<MusicSymbol*>* Measure::getLeft() {
	return &left;
}

vector<MusicSymbol*>* Measure::getRight() {
	return &right;
}

Measure::status Measure::getStatus(MusicSymbol* m) {
	Duration symbolDuration = m->getDuration();

	if (currDur + symbolDuration <= maxDur) return OK;

	if (currDur == maxDur) return FULL;

	return SPLIT;
}

void Measure::addDuration(Duration d) {
	currDur = currDur + d;
}

ostream& operator<<(ostream& os, const Measure& m) {
	os << "L: ";
	for (int i = 0; i < m.left.size(); i++) {
		os << *m.left[i] << " ";
	}
	os << endl;
	os << "R: ";
	for (int i = 0; i < m.right.size(); i++) {
		os << *m.right[i] << " ";
	}
	os << endl;
	return os;
}