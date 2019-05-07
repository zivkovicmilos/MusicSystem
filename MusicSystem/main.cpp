#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <map>
#include <iterator>
#include <utility>
#include <regex>
#include <fstream>
#include "MidiFile.h"
#include "vector"
#include "Duration.h"
#include "MusicSymbol.h"
#include "Note.h"
#include "Pause.h"
#include "Composition.h"
#include "MXMLFormatter.h"
#include "MIDIFormatter.h"
#include "BMPFormatter.h"
#include "CompIterator.h"
using namespace std;

void exportText() {
	cout << endl;
	cout << "\n  * * * * * * * * * * * * * * * * * * *" << endl;
	cout << "  *            Export Menu	      *" << endl;
	cout << "  * * * * * * * * * * * * * * * * * * *" << endl;
	cout << "  *                                   *" << endl;
	cout << "  * 1. Export as MusicXML             *" << endl;
	cout << "  * 2. Export as BMP                  *" << endl;
	cout << "  * 3. Export as MIDI                 *" << endl;
	cout << "  * 4. Return to Main Menu            *" << endl;
	cout << "  *                                   *" << endl;
	cout << "  * * * * * * * * * * * * * * * * * * *" << endl;
	cout << endl;
}

void exportMenu(Composition* comp, map<string, int>& midiMap, 
				MXMLFormatter* mxml, MIDIFormatter* midi, BMPFormatter*bmp) {
	int ans = 0;
	bool inUse = true;
	while (inUse) {
		exportText();
		cout << "> ";
		cin >> ans;
		switch (ans) {
		case 1:
			// MusicXML
			if (!comp) {
				cout << "No composition loaded";
				inUse = false;
				break;
			}
			mxml = new MXMLFormatter(comp);
			mxml->format();
			break;
		case 2:
			// BMP
			if (!comp) {
				cout << "No composition loaded";
				inUse = false;
				break;
			}
			cout << "Enter the image width:\n";
			cout << "> ";
			cin >> ans;
			bmp = new BMPFormatter(comp, ans);
			bmp->format();
			break;
		case 3:
			// MIDI
			if (!comp || midiMap.size() == 0) {
				cout << "No composition / Midi map loaded";
				inUse = false;
				break;
			}
			midi = new MIDIFormatter(comp, &midiMap);
			midi->format();
			break;
		case 4:
			inUse = false;
			break;
		}
	}
}

void compositionText() {
	cout << endl;
	cout << "\n  * * * * * * * * * * * * * * * * * * *" << endl;
	cout << "  *         Composition Menu	      *" << endl;
	cout << "  * * * * * * * * * * * * * * * * * * *" << endl;
	cout << "  *                                   *" << endl;
	cout << "  * 1. Print Composition              *" << endl;
	cout << "  * 2. Iterate Composition            *" << endl;
	cout << "  * 3. Change Duration                *" << endl;
	cout << "  * 4. Increase / Decrease octaves    *" << endl;
	cout << "  * 5. Return to Main Menu            *" << endl;
	cout << "  *                                   *" << endl;
	cout << "  * * * * * * * * * * * * * * * * * * *" << endl;
	cout << endl;
}

void noteIterMenu() {
	cout << endl;
	cout << "1. Previous Note" << endl;
	cout << "2. Next Note" << endl;
	cout << "3. Change Pitch" << endl;
	cout << "4. Change Octave" << endl;
	cout << "5. Make Sharp" << endl;
	cout << "6. Remove Sharp" << endl;
	cout << "7. Go back" << endl;
	cout << "> ";
}

void compIterMenu() {
	cout << endl;
	cout << "1. Previous Measure" << endl;
	cout << "2. Next Measure" << endl;
	cout << "3. Iterate Notes" << endl;
	cout << "4. Go back" << endl;
	cout << "> ";
}

void compositionMenu(Composition* comp, bool& changed) {
	
	bool inUse = true;
	bool compIterating = true;
	bool noteIterating = false;
	int curr = 1;
	int ans = 0;
	int numerator = 0;
	vector<Measure*>* measureArr = comp->getMeasureArr();
	vector<Note*>* noteArr = comp->getNoteArr();

	vector<Measure*>::iterator compIt = measureArr->begin();
	vector<Note*>::iterator noteIt = noteArr->begin();
	int denominator = 0;

	while (inUse) {
		if (!comp) {
			cout << "No composition loaded\n";
			inUse = false;
			break;
		}
		compositionText();
		cout << "> ";
		cin >> ans;
		switch (ans) {
		case 1:
			// TODO Fix output
			cout << *comp;
			break;
		case 2:
			compIterating = true;
			while (compIterating) {
				cout << "Measure " << curr << ":" << endl;
				cout << **compIt; // Print current measure
				compIterMenu();
				cin >> ans;
				switch (ans) {
				case 1:
					if (compIt != measureArr->begin()) {
						--compIt;
						curr--;
					}
					break;
				case 2:
					if (compIt != measureArr->end()) {
						compIt++;
						curr++;
					}
					break;
				case 3:
					noteIterating = true;
					while (noteIterating) {
						cout << "Note: " << **noteIt;
						noteIterMenu();
						cin >> ans;
						switch (ans) {
						case 1:
							if (noteIt != noteArr->begin()) {
							--noteIt;
							}
							break;
						case 2:
							if (noteIt != noteArr->end()) {
								noteIt++;
							}
							break;
						case 3:// TODO iterate through chained notes
							cout << "Enter the new pitch: " << endl;
							cout << "> ";
							char pitch;
							cin >> pitch;
							(*noteIt)->changePitch(pitch);
							break;
						case 4:
							cout << "Enter the new octave: " << endl;
							cout << "> ";
							int octave;
							cin >> octave;
							(*noteIt)->changeOctave(octave);
							break;
						case 5:
							(*noteIt)->setSharp();
							break;
						case 6:
							(*noteIt)->removeSharp();
							break;
						case 7:
							noteIterating = false;
							noteIt = noteArr->begin();
							break;
						}

					}
					break;
				case 4:
					compIterating = false;
					compIt = measureArr->begin();
					curr = 1;
					break;
				}
			}
			break;
		case 3:
			cout << "\nEnter a new duration (numerator denominator):\n";
			cout << "> ";
			cin >> numerator;
			cin >> denominator;
			try {
				Duration d(numerator, denominator);
			}
			catch (InvalidDenominator id) {
				cout << id.what();
				break;
			}
			comp->changeDuration(Duration(numerator, denominator));
			changed = true;
			compIt = comp->getMeasureArr()->begin();
			break;
		case 4:
			cout << "\nEnter a number (+/-):\n";
			cout << "> ";
			cin >> ans;
			comp->changeOctaves(ans);
			changed = true;
			break;
		case 5:
			inUse = false;
			break;
		}
	}
}

void mainMenu() {
	cout << endl;
	cout << "\n  * * * * * * * * * * * * * * * * * * *" << endl;
	cout << "  *            Main Menu	      *" << endl;
	cout << "  * * * * * * * * * * * * * * * * * * *" << endl;
	cout << "  *                                   *" << endl;
	cout << "  * 1. Import Note Data               *" << endl;
	cout << "  * 2. Import Composition Data        *" << endl;
	cout << "  * 3. Composition Menu               *" << endl;
	cout << "  * 4. Export Menu                    *" << endl;
	cout << "  * 5. Stop                           *" << endl;
	cout << "  *                                   *" << endl;
	cout << "  * * * * * * * * * * * * * * * * * * *" << endl;
	cout << endl;
}

int main() {
	ifstream input;
	map<char, string> noteMap;
	map<string, int> midiMap;
	Composition* comp = nullptr;
	MXMLFormatter* mxml = nullptr;
	MIDIFormatter* midi = nullptr;
	BMPFormatter* bmp = nullptr;
	int ans = 0;
	bool madeChanges = false;
	string name;
	string textLine;

	while(true) {
		mainMenu();
		cout << "> ";
		cin >> ans;
		switch (ans) {
		case 1:
			//Note Data
			cout << "Please specify a filename (*.csv):\n";
			cout << "> ";
			cin >> name;
			input.open(name + ".csv");
			if (!input.fail()) {
				regex rx("([^,]*),([^,]*),([^,]*)*");

				while (getline(input, textLine)) {
					smatch result;

					if (regex_match(textLine, result, rx)) {
						char character = (result.str(1).c_str())[0];
						string note = result.str(2);
						int midiNum = atoi(result.str(3).c_str());
						noteMap[character] = note;
						midiMap[note] = midiNum;
					}
				}
				input.close();
			}
			else {
				cout << "\nThere was an error opening the file";
			}
			break;
		case 2:
			// Composition Data
			if (noteMap.size() == 0) {
				cout << "Please load the Note data first";
				break;
			}
			cout << "Please specify a filename (*.txt):\n";
			cout << "> ";
			cin >> name;
			cout << "\nEnter the duration (numerator denominator):\n";
			int num, denom;
			cin >> num;
			cin >> denom;
			comp = new Composition(Duration(num, denom));
			if (!comp) break; // Invalid duration
			input.open("input\\" + name + ".txt");
			if (!input.fail()) {
				comp->addSymbols(noteMap, input);
				input.close();
			}
			else {
				cout << "\nThere was an error opening the file";
			}
			break;
		case 3:
			compositionMenu(comp, madeChanges);
			break;
		case 4:
			// Export
			exportMenu(comp, midiMap, mxml, midi, bmp); // check if pointers are good
			break;
		case 5:
			// Stop
			cout << "Are you sure? (y/n)";
			if (madeChanges) {
				cout << "\nChanges were not exported\n";
			}
			cin >> textLine;
			if (textLine == "y") {
				cout << "\nDo you want to export before exiting? (y/n)\n";
				cout << "> ";
				cin >> textLine;
				if (textLine == "y") {
					exportMenu(comp, midiMap, mxml, midi, bmp);
				}
				comp->~Composition();
				return 0;
			}
			break;
		}
	}
}