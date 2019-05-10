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

uint8_t BMPFormatter::getColorMix(vector<uint8_t>& colors) {
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
		return (val + ((255 - val) / 8) * 6);
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

void write8bit(std::ostream& out, char const* c, std::size_t n) {
	out.write(c, n);
}

void write8bit(ofstream& output, char num) {
	output.put(num);
}

void write8bit(std::ostream& out, byte const* c, std::size_t n) {
	return write8bit(out, reinterpret_cast<char const*>(c), n);
}

void write16bit(ofstream& output, uint16_t num) {
	output.put(num & 0xFFu).put((num >> 8) & 0xFFu);
}


void write32bit(ofstream& output, uint32_t num) {
	output.put(num & 0xFFu).put((num >> 8) & 0xFFu).put((num >> 16) & 0xFFu).put((num >> 24) & 0xFFu);
}

void BMPFormatter::generateBoilerplate(ofstream& output, uint32_t totalSize, uint32_t width, uint32_t height) {

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

}

int getClosest(int rowPixelCnt) {
	if ((rowPixelCnt % 4) == 0) return 0; // Already a multiple of 8
	if ((rowPixelCnt % 4) == rowPixelCnt) return 4 - rowPixelCnt; // num < 8

	int closest = 4 + (rowPixelCnt - (rowPixelCnt % 4)); // Closest multiple of 8
	return closest - rowPixelCnt;
}

void BMPFormatter::format() {
	if (colorMap.empty()) readColorMap(); // Load in the color map;
	
	string outputFileName;
	cout << "Enter the BMP output file name: ";
	cout << "> ";
	cin >> outputFileName;

	// Pause is white (255, 255, 255)
	// 1/4 is two pixels
	// 1/8 is one pixel
	struct PIXEL {
		uint8_t r, g, b;
		PIXEL(uint8_t r, uint8_t g, uint8_t b) {
			this->r = r;
			this->g = g;
			this->b = b;
		}
	};

	vector<PIXEL> colors;
	vector<Measure*>* measureArr = comp->getMeasureArr();
	vector<MusicSymbol*>* right = nullptr;
	vector<MusicSymbol*>* left = nullptr;

	bool splitFlag = false;
	int pixels = 0;
	int height = 1;
	int rowPixelCnt = 0;
	int totalSize = 0;

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
			PIXEL temp(getColorMix(red), getColorMix(green), getColorMix(blue));

			// Add an according number of pixels
			for (int dur = 0; dur < pixels; dur++) {
				//output << (byte)r << (byte)g << (byte)b; // One pixel
				colors.push_back(temp);

				rowPixelCnt++;
				if (rowPixelCnt == width) {
					
					int padd = getClosest(rowPixelCnt); // Number of 00 pairs
					for (int padding = 0; padding < padd; padding++) {
						//colors.push_back(0); // Maybe this is the cause of black
						totalSize++;
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
		// changed getClosest
		int remaining = width - rowPixelCnt;
		for (int i = 0; i < remaining; i++) {
			// One white pixel
			colors.push_back(PIXEL(255, 255, 255));
		}
		totalSize += (remaining * 3);
	}

	//output.close();
	ofstream output("BMP\\"+outputFileName+".bmp", std::ios::binary);
	uint32_t rowWidth = 3 * width; // How many bytes in a row
	uint8_t padding = (4 - (width * 3) % 4) % 4;

	char* buff = new char[colors.size() * 3 + 1];
	int cnt = 0;
	for (int i = 0; i < colors.size(); i++) {
		buff[cnt] = colors[i].r;
		buff[cnt+1] = colors[i].g;
		buff[cnt+2] = colors[i].b;
		cnt += 3;
	}

	generateBoilerplate(output, totalSize, width, height); // ok

	for (int i = height - 1; i >= 0; i--) {
		output.write(buff + i * rowWidth, rowWidth);
		// changed i!=0
		for (int j = 0; j < padding; j++) {
			write8bit(output, 0);
		}
	}

	delete buff;

	colors.clear();
	red.clear();
	blue.clear();
	green.clear();

	output.close();
}