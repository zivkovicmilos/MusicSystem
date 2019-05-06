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
using namespace std;

Composition* addSymbols(map<char, string>& noteMap) {
	ifstream file;
	// TODO pass in the filename
	file.open("test2.txt");
	vector<pair<MusicSymbol*, int>> playing;

	regex outside("([a-zA-Z0-9]{1,}(?![^\\[\\]]*\\]))");
	regex spaces("([ ]{1}(?![^\\[\\]]*\\]))");
	//(?<=\[)[a-zA-Z0-9 ]*(?=\])
	regex inside("\\[([^\\[\\]]*[^\\[\\]])\\]");
	regex special("(\\|)");
	vector<regex> rxs;

	rxs.push_back(outside);
	rxs.push_back(spaces);
	rxs.push_back(inside);
	rxs.push_back(special);

	int base = 0;
	int oldBase = 0;
	string textLine;
	MusicSymbol* temp = nullptr;

	while (getline(file, textLine)) {
		base = textLine.size(); // grab the offset if it's a multi-line file

		for (int i = 0; i < rxs.size(); i++) {
			regex reg = rxs[i];
			smatch match;
			bool first = true;
			int num = 0;
			int oldLength = 0;
			Note* oldNote = nullptr;
			string workingStr;
			int pos;

			string::const_iterator searchStart(textLine.cbegin());

			while (regex_search(searchStart, textLine.cend(), match, reg)) {

				workingStr = (i == 2) ? match.str(1) : match.str();
				pos = (i == 2) ? match.position(1) : match.position();
				oldNote = nullptr;

				if (first) {
					num = oldBase + pos;
					first = false;
				}
				else {
					num += oldLength + pos;
					num += (i == 2 ? -1 : 0);
				}
				// TODO: Remove below
				cout << "(" << workingStr << ") " << " at " << num << endl;
				oldLength = match.str().length();
				/////////////////////////////

				int length = workingStr.length();
				if (length > 1) {
					char* charArr = new char[length + 1];
					strcpy(charArr, workingStr.c_str());
					int offset = 0;
					bool connected = false;
					bool added = false;
					for (int j = 0; j < length; j++) {
						Duration d(0);
						if (i == 2) {
							// Capturing inside the brackets [ ]
							if (workingStr.find(' ') != string::npos) {
								// There is a space between the letters
								d.changeDuration(1, 8); // For all symbols
							} else {
								d.changeDuration(1, 4);
								connected = true;
							}
						} else {
							d.changeDuration(1, 4);
						}
						Note::Pitch pitch;
						int octave;
						bool isSharp = false;
						// TODO check if char map is valid when parsing
						char note = charArr[j];
						if (note == ' ') {
							// The symbol is a pause within [ ]
							temp = new Pause(d);
							Pause* tempP = (Pause*)temp;

							playing.push_back(pair<MusicSymbol*, int>(tempP, offset + num));
							offset++;
							continue;
						}
						char cPitch = noteMap[note][0];
						pitch = Note::getPitch(cPitch);

						if ((noteMap[note])[1] != '#') {
							octave = (int) (noteMap[note])[1] - '0';
						}
						else {
							octave = (int) (noteMap[note])[2] - '0';
							isSharp = true;
						}

						temp = new Note(d, octave, isSharp, pitch);
						
						// TODO ovde greska
						playing.push_back(pair<MusicSymbol*, int>(temp, offset + num));

						if (connected && oldNote) {
							oldNote->addNext((Note*)temp);
							Note* tmpN = (Note*)temp;
							tmpN->addPrev(oldNote);
						}
						
						offset++;
						oldNote = (Note *) temp;
					}
				}
				else {
					if (workingStr == " ") {
						temp = new Pause(Duration(1, 8));
					}
					else if (workingStr == "|") {
						temp = new Pause(Duration(1, 4));
						Pause* tempP = (Pause*)temp;
					}
					else {
						Note::Pitch pitch;
						int octave;
						bool isSharp = false;

						char cPitch = noteMap[workingStr[0]][0];
						pitch = Note::getPitch(cPitch);

						if (noteMap[workingStr[0]][1] != '#') {
							octave = (int)(noteMap[workingStr[0]])[1] - 48;

						}
						else {
							octave = (int)(noteMap[workingStr[0]])[2] - 48;
							isSharp = true;
						}

						temp = new Note(Duration(1, 4), octave, isSharp, pitch);
					}

					playing.push_back(pair<MusicSymbol*, int> (temp, num));
				}
				searchStart = match.suffix().first;
			}
		}
		oldBase = base;
	}
	// Sort the symbols
	auto lambdaRule = [] (pair<MusicSymbol*, int> m1, pair<MusicSymbol*, int> m2) {
		return m1.second < m2.second;
	};

	sort(playing.begin(), playing.end(), lambdaRule);

	// File parsed
	ofstream ofile;
	ofile.open("output.txt");
	for (int i = 0; i < playing.size(); i++) {
		MusicSymbol* t = playing[i].first;
		cout << *t << " at index: " << playing[i].second << endl;
		ofile << *t << " at index: " << playing[i].second << endl;
	}
	ofile.close();
	file.close();

	ofile.open("compositionOutput.txt");

	// TODO make the duration changeable
	Composition* c = new Composition(Duration(3, 8));

	c->attachMap(&playing);
	c->createComposition();
	cout << *c;
	ofile << *c;
	ofile.close();
	return c;
}

int main() {
	ifstream file;
	file.open("map.csv");
	string textLine;
	regex rx("([^,]*),([^,]*),([^,]*)*");
	map<char, string> noteMap;
	map<string, int> midiMap;

	while (getline(file, textLine)) {
		smatch result;

		if (regex_match(textLine, result, rx)) {
			char character = (result.str(1).c_str())[0];
			string note = result.str(2);
			int midiNum = atoi(result.str(3).c_str());
			noteMap[character] = note;
			midiMap[note] = midiNum;
		}
	}

	file.close();
	
	ofstream testFile;
	testFile.open("test.bmp");
	//testFile << (unsigned char)'B';
	//uint16_t
	//uint32_t num= 40;
	//testFile.put(num & 0xFFu).put((num >> 8) & 0xFFu).put((num >> 16) & 0xFFu).put((num >> 24) & 0xFFu);
	unsigned int num = 10;
	//testFile.write(reinterpret_cast<const char *>(&num), sizeof(num));
	//testFile.write(prvi, sizeof(prvi)).put((broj >> 8) & 0xFFu).put((broj >> 16) & 0xFFu).put((broj >> 24) & 0xFFu);

	
	Composition* comp = addSymbols(noteMap);

	MXMLFormatter* mxml = new MXMLFormatter(comp);
	mxml->format();
	
	MIDIFormatter* midi = new MIDIFormatter(comp, &midiMap);
	midi->format();

	BMPFormatter* bmp = new BMPFormatter(comp, 10);
	bmp->format();

	return 0;
}