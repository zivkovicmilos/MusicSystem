#pragma once
#include "Formatter.h"
#include "MidiFile.h"

class MIDIFormatter : public Formatter {
	map<string, int>* midiMap;
public:
	MIDIFormatter(Composition*, map<string, int>*);

	void format() override;
};