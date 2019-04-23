#pragma once
#include "MusicSymbol.h"
#include <vector>

class Measure {
	Duration maxDur;
	vector<MusicSymbol> symbols;
	bool full;
	Duration currDur;

public:
	Measure();
	// Watch out for the splitting of notes between two measures
	MusicSymbol addSymbol(MusicSymbol);
};

