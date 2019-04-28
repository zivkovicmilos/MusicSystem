#include "Composition.h"

Composition::Composition(Duration d) : d(d){
	symbolMap = nullptr;
	numOfMeasures = 0;
	//leftFirst = rightFirst = nullptr;
	//leftLast = rightLast = nullptr;
}

void Composition::attachMap(vector<pair<MusicSymbol*, int>>* sm) {
	symbolMap = sm;
	left.push_back(new Measure(d));
	right.push_back(new Measure(d));
}

void Composition::parallelAdd(vector<Measure*>& firstV, vector<Measure*>& secondV,
			MusicSymbol* first, MusicSymbol* second) {
	if (firstV[firstV.size() - 1]->isFull()) {
		// The last measure added is full, so a new one must be created
		firstV.push_back(new Measure(d));
		secondV.push_back(new Measure(d));

		firstV[firstV.size() - 1]->addSymbol(first);
		secondV[secondV.size() - 1]->addSymbol(second);

	}
	else if (firstV[firstV.size() - 1]->addSymbol(first) == Measure::status::SPLIT) {

		secondV[secondV.size() - 1]->addSymbol(second); // This returns a split ack

		firstV.push_back(new Measure(d));
		secondV.push_back(new Measure(d));

		firstV[firstV.size() - 1]->addSymbol(first);
		secondV[secondV.size() - 1]->addSymbol(second);
	}
	else {
		// No split needed
		secondV[secondV.size() - 1]->addSymbol(second);
	}
}

void Composition::createComposition() {
	for (int i = 0; i < symbolMap->size(); i++) {
		MusicSymbol* curr = (*symbolMap)[i].first;
		if (curr->checkPause()) {
			Pause* p = (Pause*)curr;
				//If it is a split, 
				//the symbol will be added to the array before returning
			parallelAdd(left, right, p, p);
		}
		else {
			// This is a note, not a pause
			Note* n = (Note*)curr;

			// Add only the first note in the sequence so it occupies one space
			if (n->getNext() && !n->getPrev()) {
				// These notes are played together
				Note* temp = n;
				Pause* p = new Pause(n->getDuration());
					if (n->getOctave() > 3) {
						// Place it in the right system
						// and place a pause in the left one
						parallelAdd(right, left, temp, p);
					}
					else {
						// TODO: Extract the below code as a separate function
						// Place it in the left system
						// and place a pause in the right one
						parallelAdd(left, right, temp, p);
					}
			}
			else if (!n->getPrev()){
				// Only one note is played
				Pause* p = new Pause(n->getDuration());
				if (n->getOctave() > 3) {
					// Place it in the right system
					// and place a pause in the left one
					parallelAdd(right, left, n, p);
				}
				else {
					// TODO: Extract the below code as a separate function
					// Place it in the left system
					// and place a pause in the right one
					parallelAdd(left, right, n, p);
				}
			}
		}
	}
	// TODO TEMPORARY
	Pause* pauza = new Pause(Duration(1, 8));
	parallelAdd(left, right, pauza, pauza);
}

ostream& operator<<(ostream& os, const Composition& c) {
	for (int i = 0; i < c.left.size(); i++) {
		os << "Measure " << i << ":" << endl;
		os << "L: " << *c.left[i] << endl;
		os << "R: " << *c.right[i] << endl;
		os << endl;
	}
	return os;
}

Duration Composition::getDuration() const {
	return d;
}

vector<Measure*>* Composition::getRight() {
	return &right;
}

vector<Measure*>* Composition::getLeft() {
	return &left;
}