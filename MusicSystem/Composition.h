#pragma once
#include "Measure.h"
#include <utility>

class Composition {
	struct Elem {
		Elem* next;
		Elem* prev;
		Measure* m;
		Elem(Measure*);
	};
	// First item in the pair is the start,
	// the second item is the end of that part
	pair<Elem*, Elem*> left;
	pair<Elem*, Elem*> right;
	Duration d;
	
	Composition(Duration);
};