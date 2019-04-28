#pragma once
#include "MusicSymbol.h"
#include "Note.h"
#include <vector>

class Measure {
	Duration maxDur;
	vector<MusicSymbol*> symbols;
	bool full;
	Duration currDur;

public:
	enum status {OK, SPLIT};
	Measure(Duration);

	status addSymbol(MusicSymbol*);
	vector<MusicSymbol*>* getSymbols();
	void incCurrent(Duration d);
	bool isFull() const;
	friend ostream& operator<<(ostream&, const Measure&);
};

