#include "MIDIFormatter.h"

MIDIFormatter::MIDIFormatter(Composition* comp, map<string, int>* mm) : Formatter(comp) {
	midiMap = mm;
}

using namespace smf;

void MIDIFormatter::format() {

	MidiFile outputfile;
	outputfile.absoluteTicks();
	vector<uchar> midievent;
	midievent.resize(3);
	int tpq = 48;
	outputfile.setTicksPerQuarterNote(tpq);
	outputfile.addTrack(1);

	int actiontime = 0;
	int actionOffset = 0;
	int duration;
	int midiNum;
	midievent[2] = 64;

	vector<Measure*>* measureArr = comp->getMeasureArr();
	vector<MusicSymbol*>* right = nullptr;
	vector<MusicSymbol*>* left = nullptr;

	for (int i = 0; i < measureArr->size(); i++) {

		actiontime = actionOffset;

		right = (*measureArr)[i]->getRight();
		left = (*measureArr)[i]->getLeft();

		// RIGHT
		for (int j = 0; j < right->size(); j++) {
			MusicSymbol* ms = (*right)[j];
			duration = ms->getDuration() == Duration(1, 4) ? 2 : 1;
			if (ms->checkPause()) {
				actiontime += tpq * duration;
				continue;
			}
			Note* n = (Note*)ms;
			Note* temp = n;
			temp->setMidiTime(actiontime, tpq);
			while (temp) {
				string note;
				note.push_back(temp->getPitchC());
				note = note + to_string(temp->getOctave());
				midiNum = (*midiMap)[note];

				midievent[0] = 0x90;
				midievent[1] = midiNum;
				outputfile.addEvent(0, temp->midiStart(), midievent);

				midievent[0] = 0x80;
				outputfile.addEvent(0, temp->midiEnd(), midievent);
				temp = temp->getNext();
			}
			actiontime += tpq * duration;
		}


		// LEFT
		actiontime = actionOffset;
		for (int j = 0; j < left->size(); j++) {
			MusicSymbol* ms = (*left)[j];
			duration = ms->getDuration() == Duration(1, 4) ? 2 : 1;
			if (ms->checkPause()) {
				actiontime += tpq * duration;
				continue;
			}
			Note* n = (Note*)ms;
			Note* temp = n;
			temp->setMidiTime(actiontime, tpq);
			while (temp) {
				string note;
				note.push_back(temp->getPitchC());
				note = note + to_string(temp->getOctave());
				midiNum = (*midiMap)[note];

				midievent[0] = 0x90;
				midievent[1] = midiNum;
				outputfile.addEvent(1, temp->midiStart(), midievent);

				midievent[0] = 0x80;
				outputfile.addEvent(1, temp->midiEnd(), midievent);
				temp = temp->getNext();
			}
			actiontime += tpq * duration;
		}

		actionOffset = actiontime;
	}

	outputfile.sortTracks();
	outputfile.write("midiOutput.mid");
}