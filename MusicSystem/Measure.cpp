#include "Measure.h"


Measure::Measure(Duration d) : maxDur(d), currDur(0), full(false) {}

Measure::status Measure::addSymbol(MusicSymbol* ms) {
	if ((currDur + ms->getDuration()) <= maxDur) {
		symbols.push_back(ms);
		if (!ms->checkPause()) {
			Note* n = (Note *)ms;
			if (n->getNext() && !n->getPrev()) {
				currDur = currDur + ms->getDuration();
			}
		}
		else {
			currDur = currDur + ms->getDuration();
		}
		return OK;
	}
	else {
		// TODO check if it fits after the split
		int num = ms->getDuration().getNumerator();
		if (!ms->checkPause()) {
			Note* n = (Note*)ms;
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
		currDur = currDur + ms->getDuration();
		return SPLIT;
	}
}