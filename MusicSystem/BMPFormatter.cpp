#include "BMPFormatter.h"
typedef unsigned char byte;

BMPFormatter::BMPFormatter(Composition* c, int width) : Formatter(c), width(width) {}

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
			colorMap[note] = tempTuple;
		}
	}
	for (auto& t : colorMap)
		std::cout << t.first << " " << get<0>(t.second) << " " << get<1>(t.second) << " " << get<2>(t.second) << "\n";

}

int BMPFormatter::getValue(string note, int color) {
	int value = 0;
	switch (color) {
		case 0: value = get<0>((colorMap)[note]); break;
		case 1: value = get<1>((colorMap)[note]); break;
		case 2: value = get<2>((colorMap)[note]); break;
	}
	return value;
}

int BMPFormatter::getColorMix(vector<int>& colors) {
	int sum = 0;
	for (int i = 0; i < colors.size(); i++) {
		sum += colors[i];
	}
	return sum / colors.size();
}

string BMPFormatter::numToHex(int num) {
	char hex[10];
	_itoa_s(num, hex, 16);
	string hexS(hex);
	transform(hexS.begin(), hexS.end(), hexS.begin(), ::toupper);
	return hexS;
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
		int tRed = get<0>((colorMap)[note]);
		int tGreen = get<1>((colorMap)[note]);
		int tBlue = get<2>((colorMap)[note]);
		pushColors(tRed, tGreen, tBlue);
		break;
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

string hexamize(string str) {
	int remaining = 0;
	string ret;
	if (str.length() % 2 == 0) {
		// Even number of digits
		remaining = 4 - str.length()/2;

		for (int i = 0; i < str.length(); i++) {
			ret.push_back(str[i]);
			if (i % 2 != 0) {
				ret.push_back(' ');
			}
		}
	}
	else {
		// Uneven number of digits
		remaining = 4 - (str.length() + 1) / 2;
		for (int i = 0; i < str.length(); i++) {
			if (i == str.length() - 1) {
				ret.push_back('0');
			}
			ret.push_back(str[i]);
			if (i % 2 != 0) {
				ret.push_back(' ');
			}
		}
	}
	if (ret[ret.length() - 1] != ' ') {
		ret.push_back(' ');
	}

	for (int i = 0; i < remaining; i++) {
		ret.push_back('0');
		ret.push_back('0');
		ret.push_back(' ');
	}

	return ret;
}

void write8bit(ofstream& output, char num) {
	output.put(num);
}

void write16bit(ofstream& output, uint16_t num) {
	output.put(num & 0xFFu).put((num >> 8) & 0xFFu);
}


void write32bit(ofstream& output, uint32_t num) {
	output.put(num & 0xFFu).put((num >> 8) & 0xFFu).put((num >> 16) & 0xFFu).put((num >> 24) & 0xFFu);
}

void BMPFormatter::generateBoilerplate(ofstream& output, int totalSize, int width, int height) {
	unsigned char headerAndDIB[55] = { 'B', 'M', totalSize + 54, 0,0,0,0,0,0,0, 54,0,0,0,40,0,0,0, 
								width, height, 1,0,0, 24, 0, 0,0,0,0, totalSize, 2835, 2835, 0,0,0,0,0,0,0,0};

	write8bit(output, 'B');
	write8bit(output, 'M');
	write32bit(output, totalSize + 54);
	write32bit(output, 0);
	write32bit(output, 54);
	write32bit(output, 40);
	write32bit(output, width);
	write32bit(output, height);
	write16bit(output, 1);
	write16bit(output, 24);
	write32bit(output, 0);
	write32bit(output, totalSize);
	write32bit(output, 2835);
	write32bit(output, 2835);
	write32bit(output, 0);
	write32bit(output, 0);

	//int bitmapSize = totalSize + 54;
	/*
	output << (byte) 'B' << (byte) 'M';
	output << (byte) (totalSize + 54);// hexamize(numToHex(totalSize + 54));
	for (int i = 0; i < 7; i++) {
		output << (byte) 0;
	}
	output << (byte)54 << (byte)0 << (byte)0 << (byte)0 << (byte)40;

	output << "28 00 00 00 ";
	output << hexamize(numToHex(width)); // add width
	output << hexamize(numToHex(height)); // add height
	output << "01 00 " << "18 00 " << "00 00 00 00 ";
	output << hexamize(numToHex(totalSize)); // add bitmap size;
	output << "13 0B 00 00 " << "13 0B 00 00 " << "00 00 00 00 " << "00 00 00 00 ";
	*/
}

int getClosest(int rowPixelCnt) {
	if ((rowPixelCnt % 8) == 0) return 0; // Already a multiple of 8
	if ((rowPixelCnt % 8) == rowPixelCnt) return 8 - rowPixelCnt; // num < 8

	int closest = 8 + (rowPixelCnt - (rowPixelCnt % 8)); // Closest multiple of 8
	return closest - rowPixelCnt;
}

void BMPFormatter::format() {
	readColorMap(); // Load in the color map;
	ofstream output;
	//output.open("colorStorage.txt");

	// Pause is white (255, 255, 255)
	// 1/4 is two pixels
	// 1/8 is one pixel

	vector<byte> colors;
	vector<Measure*>* measureArr = comp->getMeasureArr();
	vector<MusicSymbol*>* right = nullptr;
	vector<MusicSymbol*>* left = nullptr;

	bool splitFlag = false;
	int pixels = 0;
	int height = 1;
	int rowPixelCnt = 0;
	int totalSize = 0;

	int r, g ,b;

	for (int i = 0; i < measureArr->size(); i++) {
		
		right = (*measureArr)[i]->getRight();
		left = (*measureArr)[i]->getLeft();

		int measureSize = right->size(); // Same for both the left and right part

		for (int j = 0; j < measureSize; j++) {
			// Clear the containers
			red.clear();
			green.clear();
			blue.clear();

			// Right part
			MusicSymbol* ms = (*right)[j];
			if (splitFlag) {
				splitFlag = false;
				continue;
			}
			if (ms->isSplit()) {
				pixels = 2;
				splitFlag = true;
			}

			else {
				pixels = ms->getDuration() == Duration(1, 4) ? 2 : 1;
			}

			if (ms->checkPause()) {
				red.push_back(255);
				green.push_back(255);
				blue.push_back(255);
			}
			else {
				Note* n = (Note*)(*right)[j];
				Note* temp = n;
				while (temp) {
					// Add all the chained notes
					string noteStr = temp->getNoteStr();
					int octave = temp->getOctave();
					addColors(octave, noteStr);
					temp = temp->getNext();
				}
			}
			
			// Left part
			ms = (*left)[j];
			if (ms->checkPause()) {
				red.push_back(255);
				green.push_back(255);
				blue.push_back(255);
			}
			else {
				Note* n = (Note*)(*left)[j];
				Note* temp = n;
				while (temp) {
					// Add all the chained notes
					string noteStr = temp->getNoteStr();
					int octave = temp->getOctave();
					addColors(octave, noteStr);
					temp = temp->getNext();
				}
			}
			// Mix all of the colors
			r = getColorMix(red);
			g = getColorMix(green);
			b = getColorMix(blue);

			// Add an according number of pixels
			for (int dur = 0; dur < pixels; dur++) {
				//output << (byte)r << (byte)g << (byte)b; // One pixel
				colors.push_back(r);
				colors.push_back(g);
				colors.push_back(b);

				rowPixelCnt++;
				if (rowPixelCnt == width) {
					int padd = getClosest(rowPixelCnt); // Number of 00 pairs
					for (int padding = 0; padding < padd; padding++) {
						//output << (byte)0;
						colors.push_back(0);
						totalSize+=2;
					}
					rowPixelCnt = 0; // Reset the counter
					height++;
				}
			}
			totalSize += (pixels * 3);
		}
	}
	// Fill in the last row with white pixels if there is space
	if (rowPixelCnt > 0 && rowPixelCnt < width) {
		int remaining = getClosest(rowPixelCnt);
		for (int i = 0; i < remaining; i++) {
			//output << (byte)255 << (byte)255 << (byte)255; // One white pixel
			colors.push_back(255);
			colors.push_back(255);
			colors.push_back(255);
		}
		totalSize += (remaining * 3);
	}
	//output.close();

	output.open("bmpOutput.bmp");
	generateBoilerplate(output, totalSize, width, height);
	for (int i = 0; i < colors.size(); i++) {
		output << colors[i];
	}
	/*
	ifstream reader;
	reader.open("colorStorage.txt");
	string textLine;
	while (getline(reader, textLine)) {
		output << textLine;
	}
	*/
	output.close();
	//reader.close();
	cout << "end";

}