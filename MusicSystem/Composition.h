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
	/*
	typedef vector<Measure*>::iterator iterator;
	iterator begin() { return measureArr.begin(); }
	iterator end() { return measureArr.end(); }
	*/

	void attachMap(vector<pair<MusicSymbol*, int>>* symbolMap);
	void createComposition();
	void selectiveAdd(Measure*, MusicSymbol*, bool);
	vector<Measure*>* getMeasureArr();
	void addSymbols(map<char, string>&, ifstream&);
	vector<Note*>* getNoteArr();

	void changeOctaves(int);
	void changeDuration(Duration d);
	void updateComposition();

	Duration getDuration() const;
	friend ostream& operator<<(ostream&, const Composition&);
	~Composition();
};