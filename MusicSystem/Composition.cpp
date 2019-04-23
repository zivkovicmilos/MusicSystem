#include "Composition.h"

Composition::Elem::Elem(Measure* m) {
	next = prev = nullptr;
	this->m = m;
}

Composition::Composition(Duration d) : d(d){
	left.first = left.second = nullptr;
	right.first = right.second = nullptr;
}