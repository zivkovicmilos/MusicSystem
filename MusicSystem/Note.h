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
	Note* nextNote; // Next note to be played in [ ] section
public:
	Note(Duration, int, bool, Pitch);
	void addNext(Note*);
	static Pitch getPitch(char);
	void getInfo(ostream&) override;
	~Note();
};

