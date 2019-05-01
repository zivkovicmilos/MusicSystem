#pragma once
#include <utility>
#include <vector>
#include "Measure.h"
#include "MusicSymbol.h"
#include "Note.h"
#include "Pause.h"

class Composition {
	// Array of measures for MusicXML
	vector<Measure*> measureArr;

	vector<pair<MusicSymbol*, int>>* symbolMap;
	Duration d;
public:
	Composition(Duration);

	void attachMap(vector<pair<MusicSymbol*, int>>* symbolMap);
	void createComposition();
	void selectiveAdd(Measure*, MusicSymbol*, bool);
	vector<Measure*>* getMeasureArr();

	Duration getDuration() const;
	friend ostream& operator<<(ostream&, const Composition&);
};