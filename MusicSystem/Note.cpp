#include "Note.h"

Note::Note(Duration d, int octave, bool isSharp, Pitch p) : MusicSymbol(d) {
	if ((octave < 2) || (octave > 6)) throw InvalidOctave();
	this->octave = octave;
	this->p = p;
	isPause = false;
	added = false;
	isSplit = false;
	prevNote = nextNote = nullptr;
	this->isSharp = isSharp;
}

void Note::setNext(Note* n) {
	nextNote = n;
}

void Note::setPrev(Note* n) {
	prevNote = n;
}

void Note::resetPtr() {
	nextNote = prevNote = nullptr;
}

//Change the duration of the note(s)
void Note::splitDuration() {
	Note* temp = this;
	// Mark split only on the first note
	isSplit = true;
	while (temp) {
		temp->d.changeDuration(1, 8);
		temp = temp->nextNote;
	}
}

void Note::setAdded() {
	added = true;
}

bool Note::isAdded() {
	return added;
}

void Note::addNext(Note* n) {
	nextNote = n;
	n->addPrev(this);
}

void Note::addPrev(Note* n) {
	prevNote = n;
}

Note* Note::getNext() const {
	return nextNote;
}

Note* Note::getPrev() const {
	return prevNote;
}

void Note::getInfo(ostream& os) {
	char pitch;
	bool upperCase = false;
	if (d == Duration(1, 4)) {
		upperCase = true;
	}
	switch (p) {
	case C: upperCase ? pitch = 'C' : pitch = 'c'; break;
	case D: upperCase ? pitch = 'D' : pitch = 'd'; break;
	case E: upperCase ? pitch = 'E' : pitch = 'e'; break;
	case F: upperCase ? pitch = 'F' : pitch = 'f'; break;
	case G: upperCase ? pitch = 'G' : pitch = 'g'; break;
	case A: upperCase ? pitch = 'A' : pitch = 'a'; break;
	case B: upperCase ? pitch = 'B' : pitch = 'b'; break;
	}

	os << pitch;
	if (isSharp) {
		os << "#";
	}
	os << octave << " " << d;
	if (nextNote) {
		os << " (next note: " << nextNote->getPitchC();
		if (isSharp) {
			os << "#";
		}
		os << nextNote->getOctave() << ")";
	}
	if (prevNote) {
		os << " (prev note: " << prevNote->getPitchC();
		if (isSharp) {
			os << "#";
		}
		os << prevNote->getOctave() << ")";
	}
}

int Note::getOctave() const {
	return octave;
}

char Note::getPitchC() const {
	switch (p) {
	case C: return 'C';
	case D: return 'D';
	case E: return 'E';
	case F: return 'F';
	case G: return 'G';
	case A: return 'A';
	case B: return 'B';
	default: cout << "Fucked up"; break;// TODO throw exception
	};
}

Note::Pitch Note::getPitch(char c) {
	Pitch ret;
	switch (c) {
	case 'C': ret = Note::Pitch::C; break;
	case 'D': ret = Note::Pitch::D; break;
	case 'E': ret = Note::Pitch::E; break;
	case 'F': ret = Note::Pitch::F; break;
	case 'G': ret = Note::Pitch::G; break;
	case 'A': ret = Note::Pitch::A; break;
	case 'B': ret = Note::Pitch::B; break;
	default: cout << "Fucked up"; break;// TODO throw exception
	};
	return ret;
}

bool Note::checkSharp() const {
	return isSharp;
}

bool Note::checkSplit() const {
	return isSplit;
}

void Note::setSplit() {
	isSplit = true;
}