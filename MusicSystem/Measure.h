#pragma once
#include "MusicSymbol.h"
#include "Note.h"
#include <vector>

class Measure {
	Duration maxDur;

	vector<MusicSymbol*> left;
	vector<MusicSymbol*> right;

	Duration currDur;

public:
	enum status {OK, SPLIT, FULL, SKIP};
	Measure(Duration);

	status getStatus(MusicSymbol*);
	vector<MusicSymbol*>* getLeft();
	vector<MusicSymbol*>* getRight();
	void addDuration(Duration);

	friend ostream& operator<<(ostream&, const Measure&);
};

