#include "Measure.h"

Measure::Measure(Duration d) : maxDur(d), currDur(0), full(false) {}

Measure::status Measure::addSymbol(MusicSymbol* ms) {
	if ((currDur + ms->getDuration()) <= maxDur) {
		symbols.push_back(ms);
		return OK;
	}
	else {
		// check if it fits after the split
		Duration tempDuration = ms->getDuration();
		tempDuration.changeDuration(tempDuration.getNumerator(), 8);
		MusicSymbol* temp = new MusicSymbol(tempDuration);
		symbols.push_back(temp);
		return SPLIT;
	}
}