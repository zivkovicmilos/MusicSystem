#include "Note.h"

Note::Note(Duration d, int octave, bool isSharp, Pitch p) : MusicSymbol(d) {
	if ((octave < 2) || (octave > 6)) throw InvalidOctave();
	this->octave = octave;
	this->p = p;
	isPause = false;
	midiS = midiE = 0;
	added = false;
	prevNote = nextNote = nullptr;
	this->isSharp = isSharp;
}

void Note::setSharp() {
	isSharp = true;
}

void Note::removeSharp() {
	isSharp = false;
}

void Note::changeOctave(int num) {
	if (octave <= 3) {
		if (num < 2) {
			octave = 2;
		}
		else if (num <=3) {
			octave = num;
		}
	}
	else {
		if (num > 6) {
			octave = 6;
		}
		else if (num > 3) {
			octave = num;
		}
	}
}

void Note::addOctave(int num) {
	int currOctave = octave;
	if (currOctave + num < 2) {
		octave = 2;
	}
	else if (currOctave + num > 6) {
		octave = 6;
	}
	else {
		octave += num;
	}
}

void Note::changePitch(char p) {
	if (!(p != 'C' || p != 'D' || p != 'E' || p != 'F' || p != 'G' || p != 'A' || p != 'B')) return;
	this->p = getPitch(p);
}

string Note::getNoteStr() {
	string note;
	switch (p) {
	case C: note.push_back('C');  break;
	case D: note.push_back('D');  break;
	case E: note.push_back('E');  break;
	case F: note.push_back('F');  break;
	case G: note.push_back('G');  break;
	case A: note.push_back('A');  break;
	case B: note.push_back('B');  break;
	}
	if (isSharp) {
		note.push_back('#');
	}
	return note;
}

void Note::setNext(Note* n) {
	nextNote = n;
}

void Note::setMidiTime(int actionTime, int tpq, int duration) {
	Note* temp = this;
	int end = actionTime + tpq/2 * duration;
	while (temp) {
		temp->midiS = actionTime;
		temp->midiE = end;
		temp = temp->nextNote;
	}
}

int Note::midiStart() {
	return midiS;
}

int Note::midiEnd() {
	return midiE;
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
	split = true;
	while (temp) {
		temp->d.changeDuration(1, 8);
		temp = temp->nextNote;
	}
}

void Note::setAdded() {
	added = true;
}

bool Note::isAdded() const {
	return added;
}

void Note::addNext(Note* n) {
	nextNote = n;
}

void Note::clearAdded() {
	added = false;
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
	char pitch = getPitchS();

	os << pitch;
	if (isSharp) {
		os << "#";
	}
	os << octave;
	Note* temp = this->nextNote;
	while (temp) {
		os << temp->getPitchS();
		if (temp->isSharp) {
			os << "#";
		}
		os << temp->getOctave();
		temp = temp->nextNote;
	}
}

void Note::singleNote(ostream& os) {
	char pitch = getPitchS();

	os << pitch;
	if (isSharp) {
		os << "#";
	}
	os << octave;
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
	default: cout << "Bad Pitch"; break;// TODO throw exception
	};
}

char Note::getPitchS() const {
	bool upperCase = false;
	char pitch;

	if (d == Duration(1, 4) && !isSplit()) {
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
	return pitch;
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
	default: cout << "Bad Pitch"; break;// TODO throw exception
	};
	return ret;
}

bool Note::checkSharp() const {
	return isSharp;
}
// TODO remove below (2)
bool Note::checkSplit() const {
	return split;
}

Note::~Note() {
	Note* temp = nextNote;
	Note* prev = nullptr;
	while (temp) {
		prev = temp;
		temp = temp->nextNote;
		temp->prevNote = nullptr;
		delete prev;
	}

	nextNote = nullptr;
	prevNote = nullptr;
}