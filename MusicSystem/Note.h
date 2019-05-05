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
	bool added;
	Note* nextNote; // Next note to be played in [ ] section
	Note* prevNote;

	// Midi actionTime start and end
	int midiS;
	int midiE;
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
	void setNext(Note*);
	void setPrev(Note*);
	void resetPtr();
	void setAdded();
	bool isAdded();
	void splitDuration() override;
	void setMidiTime(int, int, int);

	int midiStart();
	int midiEnd();
	string getNoteStr();

	bool checkSharp() const;
	bool checkSplit() const;
	~Note();
};

