#include "Composition.h"
#include <queue>

Composition::Composition(Duration d) : d(d){
	symbolMap = nullptr;
}

//vector<pair<MusicSymbol*, int>>* Composition::getSymbolMap

void Composition::attachMap(vector<pair<MusicSymbol*, int>>* sm) {
	symbolMap = sm;
	// Add in the initial measure
	measureArr.push_back(new Measure(d));
}

vector<Measure*>* Composition::getMeasureArr() {
	return &measureArr;
}

void addToList(Note*& first, Note* temp) {
	if (!first) {
		first = temp;
	}
	else {
		Note* last = first;
		while (last->getNext()) {
			last = last->getNext();
		}
		last->addNext(temp);
		temp->addPrev(last);
	}
}

/*
There is only one flag on the notes to mark them as split
When outputting the MXML file, this flag is checked and accordingly handled.
*/

void Composition::selectiveAdd(Measure* m, MusicSymbol* ms, bool split) {

	vector<MusicSymbol*>* left = m->getLeft();
	vector<MusicSymbol*>* right = m->getRight();

	vector<MusicSymbol*>* nextLeft = nullptr;
	vector<MusicSymbol*>* nextRight = nullptr;

	if (split) {
		nextLeft = getMeasureArr()->back()->getLeft();
		nextRight = getMeasureArr()->back()->getRight();
	}

	if (ms->checkPause()) {
		// A pause goes in both systems
		Pause* p = (Pause*)ms;
		left->push_back(p);
		right->push_back(p);
		if (split) {
			nextLeft->push_back(p);
			nextRight->push_back(p);
		}

		m->addDuration(p->getDuration());
		return;
	}

	Note* note = (Note*)ms;
	if (note->isAdded()) return;

	if (note->getNext()) {
		// There are multiple notes

		std::queue <Note*> together;
		Note* temp = note;
		Note* oldNext = nullptr;
		while (temp) {
			together.push(temp);
			oldNext = temp;
			temp = temp->getNext();
			oldNext->resetPtr();
		}

		Note* leftNotes = nullptr;
		Note* rightNotes = nullptr;
		while (!together.empty()) {
			temp = together.front();
			temp->setAdded();
			together.pop();
			if (temp->getOctave() > 3) {
				addToList(rightNotes, temp);
			}
			else {
				addToList(leftNotes, temp);
			}
		}
		
		// Add the notes accordingly
		if (leftNotes && rightNotes) {
			// Both systems have notes waiting
			left->push_back(leftNotes);
			right->push_back(rightNotes);

			if (split) {
				nextLeft->push_back(leftNotes);
				nextRight->push_back(rightNotes);
			}
		}
		else if (!leftNotes && rightNotes) {
			left->push_back(new Pause(note->getDuration()));
			right->push_back(rightNotes);

			if (split) {
				nextLeft->push_back(new Pause(note->getDuration()));
				nextRight->push_back(rightNotes);
			}
		}
		else {
			// The right system is empty
			left->push_back(leftNotes);
			right->push_back(new Pause(note->getDuration()));

			if (split) {
				nextLeft->push_back(leftNotes);
				nextRight->push_back(new Pause(note->getDuration()));
			}
		}
	}
	else {
		// Just add the one note to the correct system
		if (note->getOctave() > 3) {
			right->push_back(note);
			left->push_back(new Pause(note->getDuration()));

			if (split) {
				nextRight->push_back(note);
				nextLeft->push_back(new Pause(note->getDuration()));
			}
		}
		else {
			left->push_back(note);
			right->push_back(new Pause(note->getDuration()));

			if (split) {
				nextLeft->push_back(note);
				nextRight->push_back(new Pause(note->getDuration()));
			}
		}
	}
	
	m->addDuration(note->getDuration());
	if (split) {
		getMeasureArr()->back()->addDuration(note->getDuration());
	}
}

void Composition::createComposition() {

	MusicSymbol* ms = nullptr;
	for (int i = 0; i < symbolMap->size(); i++) {
		ms = (*symbolMap)[i].first;
		
		switch (measureArr.back()->getStatus(ms)) {
		case Measure::status::OK:
			// There is room for the current note(s)
			selectiveAdd(measureArr.back(), ms, false);
			break;
		case Measure::status::FULL:
			// There is no room for the current note(s), a new measure is needed
			measureArr.push_back(new Measure(d));
			selectiveAdd(measureArr.back(), ms, false);
			break;
		case Measure::status::SPLIT :{
			// A split is required to fit the note(s)
			ms->splitDuration();
			Measure* temp = measureArr.back();
			measureArr.push_back(new Measure(d));

			// Add the split note group to both measures
			selectiveAdd(temp, ms, true);
			break;
		}
		case Measure::status::SKIP:
			break;
		}
	}
}
ostream& operator<<(ostream& os, const Composition& c) {
	Measure* currMeasure = nullptr;
	for (int i = 0; i < c.measureArr.size(); i++) {
		currMeasure = c.measureArr[i];
		os << "Measure " << i << ":" << endl;
		os << *currMeasure;
		os << endl;
	}
	return os;
}

Duration Composition::getDuration() const {
	return d;
}