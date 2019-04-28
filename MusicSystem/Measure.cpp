#include "Measure.h"


Measure::Measure(Duration d) : maxDur(d), currDur(0), full(false) {}

void Measure::incCurrent(Duration d) {
	currDur = currDur + d;
	if (currDur == maxDur) {
		full = true;
	}
}

bool Measure::isFull() const {
	return full;
}

Measure::status Measure::addSymbol(MusicSymbol* ms) {
	if ((currDur + ms->getDuration()) <= maxDur) {
		symbols.push_back(ms);
		if (!ms->checkPause()) {
			Note* n = (Note *)ms;
			if (n->getNext() && !n->getPrev()) {
				incCurrent(ms->getDuration());
			}
			else {
				incCurrent(ms->getDuration());
			}
		}
		else {
			incCurrent(ms->getDuration());
		}
		return OK;
	}
	else {
		// TODO check if it fits after the split
		int num = ms->getDuration().getNumerator();
		if (!ms->checkPause()) {
			Note* n = (Note*)ms;
			n->setSplit(); // Mark it as split
			// Set up the durations of all linked notes
			if (n->getNext() && !(n->getPrev())) {
				Note* temp = n;
				while (temp) {
					temp->setDuration(num, 8);
					temp = temp->getNext();
				}
			}
			else {
				ms->setDuration(num, 8);
			}
		}
		else {
			ms->setDuration(num, 8);
		}
		
		//tempDuration.changeDuration(tempDuration->getNumerator(), 8);
		symbols.push_back(ms);
		incCurrent(ms->getDuration());
		return SPLIT;
	}
}

ostream& operator<<(ostream& os, const Measure& m) {
	for (int i = 0; i < m.symbols.size(); i++) {
		os << *m.symbols[i] << " ";
	}
	return os;
}

vector<MusicSymbol*>* Measure::getSymbols() {
	return &symbols;
}