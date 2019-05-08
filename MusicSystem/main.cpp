#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include "Menu.h"
using namespace std;

int main() {
	ifstream input;
	map<char, string> noteMap;
	map<string, int> midiMap;
	Composition* comp = nullptr;
	MXMLFormatter* mxml = nullptr;
	MIDIFormatter* midi = nullptr;
	BMPFormatter* bmp = nullptr;
	int ans = 0;
	Menu* menu = new Menu();
	string name;
	string textLine;

	while(true) {
		menu->mainMenu();
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
			menu->compositionMenu(comp);
			break;
		case 4:
			// Export
			menu->exportMenu(comp, midiMap, mxml, midi, bmp);
			break;
		case 5:
			// Stop
			cout << "Are you sure? (y/n)";
			if (menu->changed) {
				cout << "\nChanges were not exported\n";
			}
			cin >> textLine;
			if (textLine == "y") {
				cout << "\nDo you want to export before exiting? (y/n)\n";
				cout << "> ";
				cin >> textLine;
				if (textLine == "y") {
					menu->exportMenu(comp, midiMap, mxml, midi, bmp);
				}
				comp->~Composition();
				return 0;
			}
			break;
		}
	}
}