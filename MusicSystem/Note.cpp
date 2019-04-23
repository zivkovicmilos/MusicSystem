#include "Note.h"

Note::Note(Duration d, int octave, bool isSharp, Pitch p) : MusicSymbol(d), isSharp(false) {
	if ((octave < 2) || (octave > 6)) throw InvalidOctave();
	this->octave = octave;
	this->p = p;
	isSplit = false;
	this->isSharp = isSharp;
}