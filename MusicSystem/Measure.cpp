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
	int maxSize = 0;
	int currSize = 0;
	bool first = true; // First note in sequence
	os << "R: ";
	for (int i = 0; i < m.right.size(); i++) {
		first = true;
		if (!(m.left[i]->checkPause())) {
			Note* temp = (Note*)m.left[i];
			while (temp) {
				if (first) {
					currSize+=2;
					first = false;
				}
				else {
					currSize += 2; // Pitch + octave
				}
				if (temp->checkSharp()) {
					currSize++;
				}
				temp = temp->getNext();
			}
		}
		maxSize = maxSize < currSize ? currSize : maxSize;
		//cout << *m.right[i] << setw(maxSize) << " ";// TODO Remove
		os << *m.right[i] << '\t' << " ";
	}

	os << "|";
	os << endl;

	currSize = 0;
	os << "L: ";
	for (int i = 0; i < m.left.size(); i++) {
		first = true;
		if (!(m.right[i]->checkPause())) {
			Note* temp = (Note*)m.right[i];
			while (temp) {
				if (first) {
					currSize+=2;
					first = false;
				}
				else {
					currSize += 2; // Pitch + octave
				}
				if (temp->checkSharp()) {
					currSize++;
				}
				temp = temp->getNext();
			}
		}
		maxSize = maxSize < currSize ? currSize : maxSize;
		//cout << *m.left[i] << setw(maxSize) << " "; // TODO Remove
		os << *m.left[i] << '\t' << " ";
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