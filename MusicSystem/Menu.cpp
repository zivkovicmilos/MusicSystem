#include "Menu.h"

void Menu::mainMenu() {
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

void Menu::compIterMenu() {
	cout << endl;
	cout << "1. Previous Measure" << endl;
	cout << "2. Next Measure" << endl;
	cout << "3. Iterate Notes" << endl;
	cout << "4. Go back" << endl;
	cout << "> ";
}

void Menu::noteIterMenu() {
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

void Menu::compositionText() {
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

void Menu::exportText() {
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

void Menu::exportMenu(Composition* comp, map<string, int>& midiMap,
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

void Menu::compositionMenu(Composition* comp) {

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
			measureArr = comp->getMeasureArr();
			compIt = measureArr->begin();
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
					noteArr = comp->getNoteArr();
					noteIt = noteArr->begin();
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