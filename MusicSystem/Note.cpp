#include "Note.h"

Note::Note(Duration d, int octave, Pitch p) : MusicSymbol(d){
	if ((octave < 2) || (octave > 6)) throw InvalidOctave();
	this->octave = octave;
	this->p = p;
}