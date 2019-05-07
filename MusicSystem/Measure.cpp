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

	if (!m->checkPause()) {
		Note* n = (Note*)m;
		if (n->isAdded()) {
			return SKIP;
		}
	}

	if (currDur + symbolDuration <= maxDur) return OK;

	if (currDur == maxDur) return FULL;

	return SPLIT;
}

void Measure::addDuration(Duration d) {
	currDur = currDur + d;
}

ostream& operator<<(ostream& os, const Measure& m) {
	int numOfSpaces = 0;
	bool first = true; // First note in sequence
	os << "R: ";
	for (int i = 0; i < m.right.size(); i++) {
		first = true;
		if (!(m.left[i]->checkPause())) {
			Note* temp = (Note*)m.left[i];
			while (temp) {
				if (first) {
					numOfSpaces++;
					first = false;
				}
				else {
					numOfSpaces += 2; // Pitch + octave
				}
				if (temp->checkSharp()) {
					numOfSpaces++;
				}
				temp = temp->getNext();
			}
		}
		os << *m.right[i];
		for (int j = 0; j < numOfSpaces + 1; j++) {
			os << " ";
		}
	}
	os << "|";
	os << endl;
	numOfSpaces = 0;
	os << "L: ";
	for (int i = 0; i < m.left.size(); i++) {
		first = true;
		if (!(m.right[i]->checkPause())) {
			Note* temp = (Note*)m.right[i];
			while (temp) {
				if (first) {
					numOfSpaces++;
					first = false;
				}
				else {
					numOfSpaces += 2; // Pitch + octave
				}
				if (temp->checkSharp()) {
					numOfSpaces++;
				}
				temp = temp->getNext();
			}
		}
		os << *m.left[i];
		numOfSpaces = numOfSpaces == 1 ? 1 : numOfSpaces + 1;
		for (int j = 0; j < numOfSpaces; j++) {
			os << " ";
		}
	}
	os << "|";
	os << endl;
	return os;
}

Measure::~Measure() {
	for (int i = 0; i < left.size(); i++) {
		if (!left[i]->isSplit()) delete left[i];
		if (!right[i]->isSplit()) delete right[i];
	}
	left.clear();
	right.clear();
}