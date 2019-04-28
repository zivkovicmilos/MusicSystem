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
	Note* nextNote; // Next note to be played in [ ] section
	Note* prevNote;
public:
	Note(Duration, int, bool, Pitch);
	void addNext(Note*);
	void addPrev(Note*);

	static Pitch getPitch(char);
	void getInfo(ostream&) override;
	int getOctave() const;
	char getPitchC() const;
	Note* getNext() const;
	Note* getPrev() const;
	void setSplit();

	bool checkSharp() const;
	bool checkSplit() const;
	~Note();
};

