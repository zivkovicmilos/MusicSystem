#pragma once
#include "MusicSymbol.h"

class Note : public MusicSymbol {
public:
	enum Pitch {C, D, E, F, G, A, B};
private:
	int octave;
	Pitch p;
	bool isSharp;
	bool added;
	Note* nextNote; // Next note to be played in [ ] section
	Note* prevNote; // Previous note in the [ ] section

	// Midi actionTime start / end
	int midiS;
	int midiE;
public:
	Note(Duration, int, bool, Pitch);
	void addNext(Note*);
	void addPrev(Note*);

	static Pitch getPitch(char);
	void getInfo(ostream&) override;
	void singleNote(ostream&);
	int getOctave() const;
	char getPitchC() const;
	char getPitchS() const;
	
	Note* getNext() const;
	Note* getPrev() const;

	void setNext(Note*);
	void setPrev(Note*);
	void resetPtr();
	void setAdded();
	void splitDuration() override;
	void setMidiTime(int, int, int);

	int midiStart();
	int midiEnd();
	string getNoteStr();
	void changeOctave(int); // Changes the current octave to a specified number
	void addOctave(int); // Offsets the current octave by a specified number
	void changePitch(char);
	void setSharp();
	void removeSharp();

	bool checkSharp() const;
	bool checkSplit() const;
	bool isAdded() const;
	void clearAdded();
	~Note();
};

