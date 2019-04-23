#pragma once
#include "MusicSymbol.h"

class Note : public MusicSymbol {
public:
	enum Pitch {C, D, E, F, G, A, B};
private:
	// somehow mark it as split
	int octave;
	Pitch p;
	bool isSharp;
	bool isSplit;
public:
	Note(Duration, int, bool, Pitch);
	~Note();
};

