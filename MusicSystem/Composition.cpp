#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include "Composition.h"
#include <queue>

Composition::Composition(Duration d) : d(d){
	measureArr.push_back(new Measure(d));
}

void Composition::updateComposition() {
	measureArr.clear();
	measureArr.push_back(new Measure(this->d));
	createComposition();
}

void Composition::changeOctaves(int num) {
	for (int i = 0; i < symbolMap.size(); i++) {
		MusicSymbol* ms = symbolMap[i].first;
		if (ms->checkPause()) {
			continue;
		}
		Note* n = (Note*)ms;
		n->addOctave(num);
	}
	// Update the composition
	updateComposition();
}

void Composition::changeDuration(Duration d) {
	this->d = d;
	// Update composition
	updateComposition();
}

vector<Measure*>* Composition::getMeasureArr() {
	return &measureArr;
}

void addToList(Note*& first, Note* temp) {
	if (!first) {
		first = temp;
	}
	else {
		Note* last = first;
		while (last->getNext()) {
			last = last->getNext();
		}
		last->addNext(temp);
		temp->addPrev(last);
	}
}

void Composition::selectiveAdd(Measure* m, MusicSymbol* ms, bool split) {

	vector<MusicSymbol*>* left = m->getLeft();
	vector<MusicSymbol*>* right = m->getRight();

	vector<MusicSymbol*>* nextLeft = nullptr;
	vector<MusicSymbol*>* nextRight = nullptr;

	if (split) {
		nextLeft = getMeasureArr()->back()->getLeft();
		nextRight = getMeasureArr()->back()->getRight();
	}

	if (ms->checkPause()) {
		// A pause goes in both systems
		Pause* p = (Pause*)ms;
		left->push_back(p);
		right->push_back(p);
		if (split) {
			nextLeft->push_back(p);
			nextRight->push_back(p);
			getMeasureArr()->back()->addDuration(p->getDuration());
		}

		m->addDuration(p->getDuration());
		return;
	}

	Note* note = (Note*)ms;
	Pause* tempPause = new Pause(note->getDuration());
	if (split) {
		tempPause->setSplit();
	}

	if (note->isAdded()) return;

	if (note->getNext()) {
		// There are multiple notes

		std::queue <Note*> together;
		Note* temp = note;
		Note* oldNext = nullptr;
		while (temp) {
			together.push(temp);
			oldNext = temp;
			temp = temp->getNext();
			oldNext->resetPtr();
		}

		Note* leftNotes = nullptr;
		Note* rightNotes = nullptr;
		while (!together.empty()) {
			temp = together.front();
			temp->setAdded();
			together.pop();
			if (temp->getOctave() > 3) {
				addToList(rightNotes, temp);
			}
			else {
				addToList(leftNotes, temp);
			}
		}

		// Add the notes accordingly
		if (leftNotes && rightNotes) {
			// Both systems have notes waiting
			left->push_back(leftNotes);
			right->push_back(rightNotes);

			if (split) {
				nextLeft->push_back(leftNotes);
				nextRight->push_back(rightNotes);
			}
		}
		else if (!leftNotes && rightNotes) {
			left->push_back(tempPause);
			right->push_back(rightNotes);

			if (split) {
				nextLeft->push_back(tempPause);
				nextRight->push_back(rightNotes);
			}
		}
		else {
			// The right system is empty
			left->push_back(leftNotes);
			right->push_back(tempPause);

			if (split) {
				nextLeft->push_back(leftNotes);
				nextRight->push_back(tempPause);
			}
		}
	}
	else {
		// Just add the one note to the correct system
		if (note->getOctave() > 3) {
			right->push_back(note);
			left->push_back(tempPause);

			if (split) {
				nextRight->push_back(note);
				nextLeft->push_back(tempPause);
			}
		}
		else {
			left->push_back(note);
			right->push_back(tempPause);

			if (split) {
				nextLeft->push_back(note);
				nextRight->push_back(tempPause);
			}
		}
	}

	m->addDuration(note->getDuration());
	if (split) {
		getMeasureArr()->back()->addDuration(note->getDuration());
	}
}

void Composition::createComposition() {

	MusicSymbol* ms = nullptr;
	for (int i = 0; i < symbolMap.size(); i++) {
		ms = symbolMap[i].first;
		
		switch (measureArr.back()->getStatus(ms)) {
		case Measure::status::OK:
			// There is room for the current note(s)
			selectiveAdd(measureArr.back(), ms, false);
			break;
		case Measure::status::FULL:
			// There is no room for the current note(s), a new measure is needed
			measureArr.push_back(new Measure(d));
			selectiveAdd(measureArr.back(), ms, false);
			break;
		case Measure::status::SPLIT :{
			// A split is required to fit the note(s)
			ms->splitDuration();
			Measure* temp = measureArr.back();
			measureArr.push_back(new Measure(d));

			// Add the split note group to both measures
			selectiveAdd(temp, ms, true);
			break;
		}
		case Measure::status::SKIP:
			break;
		}
	}
}
ostream& operator<<(ostream& os, const Composition& c) {
	Measure* currMeasure = nullptr;
	for (int i = 0; i < c.measureArr.size(); i++) {
		currMeasure = c.measureArr[i];
		os << *currMeasure;
		os << endl;
	}
	return os;
}

Duration Composition::getDuration() const {
	return d;
}

vector<Note*>* Composition::getNoteArr() {
	noteMap.clear();
	for (int i = 0; i < symbolMap.size(); i++) {
		MusicSymbol* ms = symbolMap[i].first;
		if (ms->checkPause()) continue;
		noteMap.push_back((Note*)ms);
	}
	return &noteMap;
}

void Composition::addSymbols(map<char, string>& noteMap, ifstream& file) {
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
				//cout << "(" << workingStr << ") " << " at " << num << endl;
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
							}
							else {
								d.changeDuration(1, 4);
								connected = true;
							}
						}
						else {
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

							symbolMap.push_back(pair<MusicSymbol*, int>(tempP, offset + num));
							offset++;
							continue;
						}
						char cPitch = noteMap[note][0];
						pitch = Note::getPitch(cPitch);

						if ((noteMap[note])[1] != '#') {
							octave = (int)(noteMap[note])[1] - '0';
						}
						else {
							octave = (int)(noteMap[note])[2] - '0';
							isSharp = true;
						}

						temp = new Note(d, octave, isSharp, pitch);

						symbolMap.push_back(pair<MusicSymbol*, int>(temp, offset + num));

						if (connected && oldNote) {
							oldNote->addNext((Note*)temp);
							Note* tmpN = (Note*)temp;
							tmpN->addPrev(oldNote);
						}

						offset++;
						oldNote = (Note *)temp;
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

					symbolMap.push_back(pair<MusicSymbol*, int>(temp, num));
				}
				searchStart = match.suffix().first;
			}
		}
		oldBase = base;
	}
	// Sort the symbols
	auto lambdaRule = [](pair<MusicSymbol*, int> m1, pair<MusicSymbol*, int> m2) {
		return m1.second < m2.second;
	};

	sort(symbolMap.begin(), symbolMap.end(), lambdaRule);

	// File parsed
	ofstream ofile;
	ofile.open("output.txt");
	for (int i = 0; i < symbolMap.size(); i++) {
		MusicSymbol* t = symbolMap[i].first;
		//cout << *t << " at index: " << symbolMap[i].second << endl;
		ofile << *t << " at index: " << symbolMap[i].second << endl;
	}
	ofile.close();

	ofile.open("compositionOutput.txt");

	createComposition();
	//cout << *c;
	ofile << *this;
	ofile.close();
}

Composition::~Composition() {
	vector<MusicSymbol*>* left;
	vector<MusicSymbol*>* right;
	for (int i = 0; i < measureArr.size(); i++) {
		delete measureArr[i];
	}
	measureArr.clear();

	noteMap.clear();
}