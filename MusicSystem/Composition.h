#pragma once
#include <utility>
#include <vector>
#include "Measure.h"
#include "MusicSymbol.h"
#include "Note.h"
#include "Pause.h"

class Composition {
	// First item in the pair is the start,
	// the second item is the end of that part
	/*struct Elem {
		Elem* next;
		Measure* m;
		Elem(Measure* m) {
			this->m = m;
			next = nullptr;
		}
	};
	Elem* leftFirst;
	Elem* rightFirst;

	Elem* leftLast;
	Elem* rightLast;*/
	vector<Measure*> left;
	vector<Measure*> right;

	int numOfMeasures;
	
	vector<pair<MusicSymbol*, int>>* symbolMap;
	Duration d;
public:
	Composition(Duration);

	void attachMap(vector<pair<MusicSymbol*, int>>* symbolMap);
	void parallelAdd(vector<Measure*>&, vector<Measure*>&,
		MusicSymbol*, MusicSymbol*);
	void createComposition();

	vector<Measure*>* getRight();
	vector<Measure*>* getLeft();
	Duration getDuration() const;
	friend ostream& operator<<(ostream&, const Composition&);
};