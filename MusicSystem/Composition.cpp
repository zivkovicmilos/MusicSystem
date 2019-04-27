#include "Composition.h"

Composition::Composition(Duration d) : d(d){
	symbolMap = nullptr;
	numOfMeasures = 0;
	//leftFirst = rightFirst = nullptr;
	//leftLast = rightLast = nullptr;
}

void Composition::attachMap(vector<pair<MusicSymbol*, int>>* sm) {
	symbolMap = sm;
	left[0] = new Measure(d);
	right[0] = new Measure(d);
}

void Composition::createComposition() {
	for (int i = 0; i < symbolMap->size(); i++) {
		MusicSymbol* curr = (*symbolMap)[i].first;
		if (curr->checkPause()) {
			Pause* p = (Pause*)curr;
				//If it is a split, 
				//the symbol will be added to the array before returning
				if (left[left.size() - 1]->addSymbol(p) == Measure::status::SPLIT) {
					left[left.size()] = new Measure(d);
					right[right.size()] = new Measure(d);

					left[left.size() - 1]->addSymbol(p);
					right[right.size() - 1]->addSymbol(p);
				}
		}
		else {
			// This is a note, not a pause
			Note* n = (Note*)curr;

			if (n->getNext()) {
				// These notes are played together
				Note* temp = n;
				Pause* p = new Pause(n->getDuration());
				while (temp) {
					if (n->getOctave() > 3) {
						// Place it in the right system
						// and place a pause in the left one
						if ((right[right.size() - 1]->addSymbol(temp) == Measure::status::SPLIT)
							&& (left[left.size() - 1]->addSymbol(p) == Measure::status::SPLIT)) {

							right[right.size()] = new Measure(d);
							left[left.size()] = new Measure(d);

							right[right.size() - 1]->addSymbol(temp);
							left[left.size() - 1]->addSymbol(p);
						}
					}
					else {
						// TODO: Extract the below code as a separate function
						// Place it in the left system
						// and place a pause in the right one
						if ((left[left.size() - 1]->addSymbol(temp) == Measure::status::SPLIT)
							&& (right[right.size() - 1]->addSymbol(p) == Measure::status::SPLIT)) {

							left[left.size()] = new Measure(d);
							right[right.size()] = new Measure(d);

							left[left.size() - 1]->addSymbol(temp);
							right[right.size() - 1]->addSymbol(p);
						}
					}
					temp = temp->getNext();
				}
			}
			else {
				// Only one note is played
				Pause* p = new Pause(n->getDuration());
				if (n->getOctave() > 3) {
					// Place it in the right system
					// and place a pause in the left one
					if ((right[right.size() - 1]->addSymbol(n) == Measure::status::SPLIT)
						&& (left[left.size()-1]->addSymbol(p) == Measure::status::SPLIT)) {
						
						right[right.size()] = new Measure(d);
						left[left.size()] = new Measure(d);

						right[right.size() - 1]->addSymbol(n);
						left[left.size() - 1]->addSymbol(p);
					}
				}
				else {
					// TODO: Extract the below code as a separate function
					// Place it in the left system
					// and place a pause in the right one
					if ((left[left.size() - 1]->addSymbol(n) == Measure::status::SPLIT)
						&& (right[right.size() - 1]->addSymbol(p) == Measure::status::SPLIT)) {

						left[left.size()] = new Measure(d);
						right[right.size()] = new Measure(d);

						left[left.size() - 1]->addSymbol(n);
						right[right.size() - 1]->addSymbol(p);
					}
				}
			}
		}
	}
}