#pragma once
#include "MusicSymbol.h"
#include <vector>

class Measure {
	Duration maxDur;
	vector<MusicSymbol*> symbols;
	bool full;
	Duration currDur;

public:
	enum status {OK, SPLIT};
	Measure(Duration);

	// Watch out for the splitting of notes between two measures
	status addSymbol(MusicSymbol*);
};

