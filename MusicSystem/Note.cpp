#include "Note.h"

Note::Note(Duration d, int octave, bool isSharp, Pitch p) : MusicSymbol(d), isSharp(false) {
	if ((octave < 2) || (octave > 6)) throw InvalidOctave();
	this->octave = octave;
	this->p = p;
	isSplit = false;
	nextNote = nullptr;
	this->isSharp = isSharp;
}

void Note::addNext(Note* n) {
	nextNote = n;
}

void Note::getInfo(ostream& os) {
	char pitch;
	switch (p) {
	case C: pitch = 'C'; break;
	case D: pitch = 'D'; break;
	case E: pitch = 'E'; break;
	case F: pitch = 'F'; break;
	case G: pitch = 'G'; break;
	case A: pitch = 'A'; break;
	case B: pitch = 'B'; break;
	}

	os << pitch;
	if (isSharp) {
		os << "#";
	}
	os << octave << " " << d;
	if (nextNote) {
		os << " next note: " << *nextNote;
	}
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