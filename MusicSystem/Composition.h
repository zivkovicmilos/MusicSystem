#pragma once
#include <utility>
#include <vector>
#include <map>
#include <regex>
#include <fstream>
#include "Measure.h"
#include "MusicSymbol.h"
#include "Note.h"
#include "Pause.h"

class Composition {
	// Array of measures for MusicXML
	vector<Measure*> measureArr;

	vector<pair<MusicSymbol*, int>> symbolMap;
	vector<Note*> noteMap; // For iterating
	Duration d;
public:
	Composition(Duration);

	void createComposition();
	void selectiveAdd(Measure*, MusicSymbol*, bool);
	vector<Measure*>* getMeasureArr();
	void addSymbols(map<char, string>&, ifstream&);
	vector<Note*>* getNoteArr();

	void changeOctaves(int); // Changes all the octaves in the composition
	void changeDuration(Duration d); // Changes the duration of the composition
	void updateComposition(); // Forces a restructuring, caused by making changes to symbols

	Duration getDuration() const;
	friend ostream& operator<<(ostream&, const Composition&);
	~Composition();
};