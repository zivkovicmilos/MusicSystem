#pragma once
#include "MusicSymbol.h"

class Note : public MusicSymbol {
public:
	enum Pitch {C, D, E, F, G, A, B};
private:
	int octave;
	Pitch p;

public:
	Note(Duration, int, Pitch);
	~Note();
};

