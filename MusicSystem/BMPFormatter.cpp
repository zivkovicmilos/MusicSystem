#include "BMPFormatter.h"

BMPFormatter::BMPFormatter(Composition* c) : Formatter(c) {}

void BMPFormatter::readColorMap() {
	ifstream reader;
	map<string, tuple<int, int, int>> tempMap;
	tuple<int, int, int> tempTuple;

	reader.open("BMP\\bmpCodes.txt");
	string textLine;
	regex rx("([^,]*),([^,][0-9]*),([^,][0-9]*),([^,][0-9]*).*");

	while (getline(reader, textLine)) {
		smatch result;

		if (regex_match(textLine, result, rx)) {
			string cap = result.str(1);
			string note;
			note.push_back(cap[0]);
			if (cap[1] == '#') {
				note.push_back(cap[1]);
			}
			int red = atoi(result.str(2).c_str());
			int green = atoi(result.str(3).c_str());
			int blue = atoi(result.str(4).c_str());
			tempTuple = make_tuple(red, green, blue);
			tempMap[note] = tempTuple;
		}
	}
	for (auto& t : tempMap)
		std::cout << t.first << " " << get<0>(t.second) << " " << get<1>(t.second) << " " << get<2>(t.second) << "\n";

	colorMap = &tempMap;
}

int BMPFormatter::getValue(string note, int color) {
	int value = 0;
	switch (color) {
		case 0: value = get<0>((*colorMap)[note]); break;
		case 1: value = get<1>((*colorMap)[note]); break;
		case 2: value = get<2>((*colorMap)[note]); break;
	}
	return value;
}

int getColorMix(vector<int>& colors) {
	int sum = 0;
	for (int i = 0; i < colors.size(); i++) {
		sum += colors[i];
	}
	return sum / colors.size();
}

void BMPFormatter::pushColors(int red, int green, int blue) {
	this->red.push_back(red);
	this->green.push_back(green);
	this->blue.push_back(blue);
}

void BMPFormatter::addColors(int octave, string note) {
	// Lambda declarations
	auto x2 = [this](string note, int color) -> int {
		int val = getValue(note, color);
		return (val - (val / 8) * 6);
	};

	auto x3 = [this](string note, int color) -> int {
		int val = getValue(note, color);
		return (val - (val / 8) * 3);
	};

	auto x5 = [this](string note, int color) -> int {
		int val = getValue(note, color);
		return (val + ((255 - val) / 8) * 3);
	};

	auto x6 = [this](string note, int color) -> int {
		int val = getValue(note, color);
		return (val - ((255 - val) / 8) * 6);
	};

	switch (octave) {
	case 2: {
		pushColors(x2(note, 0), x2(note, 1), x2(note, 2));
		break;
		}
	case 3: {
		pushColors(x3(note, 0), x3(note, 1), x3(note, 2));
		break;
		}
	case 4: {

	}
	case 5: {
		pushColors(x5(note, 0), x5(note, 1), x5(note, 2));
		break;
		}
	case 6: {
		pushColors(x6(note, 0), x6(note, 1), x6(note, 2));
		break;
		}
	}
}

void BMPFormatter::format() {
	readColorMap(); // Load in the color map;
	ofstream output;
	output.open("outputFile.bmp");

	// Pause is white (255, 255, 255)
	// 1/4 is two pixels
	// 1/8 is one pixel

	vector<Measure*>* measureArr = comp->getMeasureArr();
	vector<MusicSymbol*>* right = nullptr;
	vector<MusicSymbol*>* left = nullptr;

	int r, g ,b;

	int width = 20;
	for (int i = 0; i < measureArr->size(); i++) {
		// Clear the containers
		red.clear();
		green.clear();
		blue.clear();
		
		right = (*measureArr)[i]->getRight();
		left = (*measureArr)[i]->getLeft();

		int measureSize = right->size(); // Same for both the left and right part

		for (int j = 0; j < right->size(); i++) {
			// Right part
			if ((*right)[j]->checkPause()) {
				red.push_back(255);
				green.push_back(255);
				blue.push_back(255);
			}
			else {
				// TODO Add all chained notes + don't add duplicates
				Note* n = (Note*)(*right)[j];
				string noteStr = n->getNoteStr();
				int octave = n->getOctave();
				addColors(octave, noteStr);
			}
			
			// Left part
			if ((*left)[j]->checkPause()) {
				red.push_back(255);
				green.push_back(255);
				blue.push_back(255);
			}
			else {
				Note* n = (Note*)(*left)[j];
				string noteStr = n->getNoteStr();
				int octave = n->getOctave();
				addColors(octave, noteStr);
			}
			r = getColorMix(red);
			g = getColorMix(green);
			b = getColorMix(blue);

			// Check duration, add an according number of pixels
		}

	}

}