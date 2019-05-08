#pragma once
#include "MusicSymbol.h"
#include "Note.h"
#include <vector>
#include <iomanip>

class Measure {
	Duration maxDur;

	vector<MusicSymbol*> left;
	vector<MusicSymbol*> right;

	Duration currDur;
public:
	enum status {OK, SPLIT, FULL, SKIP};
	Measure(Duration);

	status getStatus(MusicSymbol*); // Returns the status of the current measure when adding symbols
	vector<MusicSymbol*>* getLeft(); // Returns the left part of the measure
	vector<MusicSymbol*>* getRight(); // Returns the right part of the measure
	void addDuration(Duration); // Increases the duration by a certain amount

	friend ostream& operator<<(ostream&, const Measure&);
	~Measure();
};

