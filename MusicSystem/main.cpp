#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <map>
#include <iterator>
#include <utility>
#include <regex>
#include <fstream>
#include "vector"
#include "Duration.h"
#include "MusicSymbol.h"
#include "Note.h"
#include "Pause.h"
//#include "Pause.h" TODO

using namespace std;

struct Temp {
	Duration d;
	//MusicSymbol* m;
	int indx;
	char note;
	Temp(char note, int i, Duration d): d(d) {
		this->note = note;
		indx = i;
	}
};

void addSymbols(map<char, pair<string, int>> noteMap) {
	//vector<MusicSymbol*> playing;
	ifstream file;
	file.open("test2.txt");
	vector<pair<MusicSymbol*, int>> playing;

	regex outside("([a-zA-Z0-9]{1,}(?![^\\[\\]]*\\]))");
	regex spaces("([ ]{1}(?![^\\[\\]]*\\]))");
	//(?<=\[)[a-zA-Z0-9 ]*(?=\])
	regex inside("\\[([^\\[\\]]*[^\\[\\]])\\]");
	vector<regex> rxs;

	rxs.push_back(outside);
	rxs.push_back(spaces);
	rxs.push_back(inside);

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
			//string realStr = str;
			string::const_iterator searchStart(textLine.cbegin());
			//regex outside("([a-zA-Z0-9]{1,}(?![^\\[\\]]*\\]))");
			while (regex_search(searchStart, textLine.cend(), match, reg)) {
				//cout << (searchStart == textLine.cbegin() ? "" : " ") << match[0] << endl;
				workingStr = (i == 2) ? match.str(1) : match.str();
				pos = (i == 2) ? match.position(1) : match.position();

				if (first) {
					num = oldBase + pos;
					first = false;
				}
				else {
					//int np = match.position();
					//int ln = match.str().length();
					num += oldLength + pos;
					num += (i == 2 ? -1 : 0);
				}
				cout << "(" << workingStr << ") " << " at " << num << endl;
				oldLength = match.str().length();
				//"at pos " << match.position() 
				int n = workingStr.length();
				if (n > 1) {
					char* charArr = new char[n + 1];
					strcpy(charArr, workingStr.c_str());
					int offset = 0;
					bool connected = false;
					for (int j = 0; j < n; j++) {
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
							playing.push_back(pair<MusicSymbol*, int>(temp, offset + num));
							offset++;
							continue;
						}
						char cPitch = noteMap[note].first[0];
						pitch = Note::getPitch(cPitch);

						string tst = noteMap[note].first;
						if ((noteMap[note].first)[1] != '#') {
							octave = (int) (noteMap[note].first)[1] - '0';
						}
						else {
							octave = (int) (noteMap[note].first)[2] - '0';
							isSharp = true;
						}

						temp = new Note(d, octave, isSharp, pitch);
						if (connected && oldNote) {
							oldNote->addNext((Note*)temp);
							//playing[playing.size()].first->setNext(temp);
							//oldNote->setNext(temp);
						}
						//Temp* temp = new Temp(charArr[i], num + offset, Duration(1, 4));
						
						playing.push_back(pair<MusicSymbol*, int>(temp, offset+num));
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
					}
					else {
						Note::Pitch pitch;
						int octave;
						bool isSharp = false;

						char cPitch = noteMap[workingStr[0]].first[0];
						pitch = Note::getPitch(cPitch);

						if (noteMap[workingStr[0]].first[1] != '#') {
							octave = (int)(noteMap[workingStr[0]].first)[1] - 48;

						}
						else {
							octave = (int)(noteMap[workingStr[0]].first)[2] - 48;
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
	// File parsed
	/* TODO ovde puca*/
	ofstream ofile;
	ofile.open("output.txt");
	for (int i = 0; i < playing.size(); i++) {
		MusicSymbol* t = playing[i].first;
		cout << *t << " at index: " << playing[i].second << endl;
		ofile << *t << " at index: " << playing[i].second << endl;
	}
	ofile.close();
	file.close();
}

int main() {
	ifstream file;
	file.open("map.csv");
	string textLine;
	regex rx("([^,]*),([^,]*),([^,]*)*");
	map<char, pair<string, int>> noteMap;
	vector<Temp*> allSymbols;

	while (getline(file, textLine)) {
		smatch result;

		if (regex_match(textLine, result, rx)) {
			char character = (result.str(1).c_str())[0];
			string note = result.str(2);
			int midiNum = atoi(result.str(3).c_str());
			noteMap[character] = pair<string, int>(note, midiNum);
			cout << character << " => " << note << ", " << midiNum << endl;
		}
	}

	file.close();
	//ifstream* file2;
	//file2.open("‪C:\\Users\\Milos\\Desktop\\test.txt");

	//file2->open("test.txt");
	regex outside("([a-zA-Z0-9]{1,}(?![^\\[\\]]*\\]))");
	regex spaces("([ ]{1}(?![^\\[\\]]*\\]))");
	addSymbols(noteMap);
	
	return 0;
}