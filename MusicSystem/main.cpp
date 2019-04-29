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
#include "Composition.h"
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

Composition* addSymbols(map<char, pair<string, int>> noteMap) {
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
				oldNote = nullptr;

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
					bool added = false;
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
							Pause* tempP = (Pause*)temp;

							playing.push_back(pair<MusicSymbol*, int>(tempP, offset + num));
							offset++;
							continue;
						}
						char cPitch = noteMap[note].first[0];
						pitch = Note::getPitch(cPitch);

						if ((noteMap[note].first)[1] != '#') {
							octave = (int) (noteMap[note].first)[1] - '0';
						}
						else {
							octave = (int) (noteMap[note].first)[2] - '0';
							isSharp = true;
						}

						temp = new Note(d, octave, isSharp, pitch);
						
						// TODO ovde greska
						playing.push_back(pair<MusicSymbol*, int>(temp, offset + num));

						if (connected && oldNote) {
							oldNote->addNext((Note*)temp);
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
						// TODO Capture this with a regex, turn off the pause regex
						temp = new Pause(Duration(1, 4));
						Pause* tempP = (Pause*)temp;
						tempP->setSpecial(); // Mark it as playable TODO Remove
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
		//cout << *t << " at index: " << playing[i].second << endl;
		ofile << *t << " at index: " << playing[i].second << endl;
	}
	ofile.close();
	file.close();

	ofile.open("compositionOutput.txt");

	Composition* c = new Composition(Duration(3, 8));
	c->attachMap(&playing);
	c->createComposition();
	cout << *c;
	ofile << *c;
	ofile.close();
	return c;
}

void additionalNotes(ostream& os, Note* n, int duration) {
	os << "<note>" << endl;
	os << "<chord/>" << endl;
	os << "<pitch>" << endl;
	os << "<step>" << n->getPitchC() << "</step>" << endl;
	os << "<octave>" << n->getOctave() << "</octave>" << endl;
	if (n->checkSharp()) {
		os << "<alter>1</alter>" << endl;
	}
	os << "</pitch>" << endl;
	os << "<duration>" << duration << "</duration>" << endl;
	os << "</note>" << endl;
}

void printNote(ostream& os, MusicSymbol* m, bool& split) {

	int duration = m->getDuration().getDenominator() == 4 ? 2 : 1;
	os << "<note>" << endl;
	if (m->checkPause()) {
		Pause* p = (Pause*)m;
		os << "<rest/>" << endl;
		os << "<duration>" << duration << "</duration>" << endl;
		os << "</note>" << endl;
	}
	else {
		Note* n = (Note*)m;
		os << "<pitch>" << endl;
		os << "<step>" << n->getPitchC() << "</step>" << endl;
		os << "<octave>" << n->getOctave() << "</octave>" << endl;
		if (n->checkSharp()) {
			os << "<alter>1</alter>" << endl;
		}
		os << "</pitch>" << endl;
		os << "<duration>" << duration << "</duration>" << endl;
		if (n->checkSplit() && !split) {
			// TODO Split arc for ALL notes
			os << "<tie type=\"start\"/>" << endl;
			split = true;
		}
		else if (split) {
			os << "<tie type=\"end\"/>" << endl;
			split = false;
		}
		os << "</note>" << endl;

		if (n->getNext()) {
			Note* temp = n->getNext();
			while (temp) {
				additionalNotes(os, temp, duration);
				temp = temp->getNext();
			}
		}
	}
}

void musicXML(Composition* c) {
	ifstream reader;
	ofstream output;
	
	bool openMeasure = true;
	
	reader.open("MusicXML\\start.txt");
	output.open("MusicXML\\output.musicxml");

	string textLine;
	while (getline(reader, textLine)) {
		output << textLine;
	}
	reader.close();
	reader.open("MusicXML\\rightBP.txt");

	output << "<beats>" << c->getDuration().getNumerator() << "</beats>" << endl;
	output << "<beat-type>" << c->getDuration().getDenominator() << "</beat-type>" << endl;

	while (getline(reader, textLine)) {
		output << textLine;
	}
	reader.close();
	// Right boilerplate done
	vector<Measure*>* measureArr = c->getMeasureArr();
	vector<MusicSymbol*>* left = nullptr;
	vector<MusicSymbol*>* right = nullptr;
	bool split = false;

	for (int i = 0; i < measureArr->size(); i++) {
		right = (*measureArr)[i]->getRight();
		if (!openMeasure) {
			output << "<measure>" << endl;
		}
		for (int j = 0; j < right->size(); j++) {
			printNote(output, (*right)[j], split);
		}
		output << "</measure>" << endl;
		openMeasure = false;
	}
	output << "</part>" << endl;
	// Right part done

	output << "<part id=\"Left\">" << endl;
	output << endl;
	output << "<measure>" << endl << "<attributes>" << endl;
	output << "<divisions>2</divisions>" << endl;
	output << "<time>" << endl;
	output << "<beats>" << c->getDuration().getNumerator() << "</beats>" << endl;
	output << "<beat-type>" << c->getDuration().getDenominator() << "</beat-type>" << endl;
	output << "</time>" << endl << "<clef>" << endl << "<sign>F</sign>" << endl << "<line>4</line>" << endl;
	output << "</clef>" << endl;
	output << "</attributes>" << endl;
	openMeasure = true;
	split = false;

	for (int i = 0; i < measureArr->size(); i++) {
		left = (*measureArr)[i]->getLeft();
		if (!openMeasure) {
			output << "<measure>" << endl;
		}
		for (int j = 0; j < left->size(); j++) {
			printNote(output, (*left)[j], split);
		}
		output << "</measure>" << endl;
		openMeasure = false;
	}
	output << "</part>" << endl;
	output << "</score-partwise>";
	output.close();
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
	Composition* c = new Composition(Duration(3, 8));
	Measure* m = new Measure(Duration(3, 8));
	Note* n1 = new Note(Duration(1, 4), 3, false, Note::Pitch::C);
	Note* n2 = new Note(Duration(1, 4), 5, false, Note::Pitch::F);
	vector<Note*> notes;
	notes.push_back(n1);
	notes.push_back(n2);

	Duration d = Duration(1, 4) + Duration(1, 8);
	cout << d << endl;
	d = Duration(1, 4) + Duration(1, 4);
	cout << d << endl;
	d = Duration(1, 8) + Duration(1, 8);
	cout << d << endl;
	d = Duration(2, 4) + Duration(1, 8);
	cout << d << endl;
	d = Duration(2, 8) + Duration(1, 4);
	cout << d << endl;
	Composition* comp = addSymbols(noteMap);
	musicXML(comp);
	return 0;
}