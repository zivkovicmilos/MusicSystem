#pragma once
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include "Formatter.h";
#include <tuple>
#include <regex>
#include <cstdint>

class BMPFormatter : Formatter {
	map<string, tuple<int, int, int>> colorMap;
	vector<uint8_t> red;
	vector<uint8_t> green;
	vector<uint8_t> blue;

	int width;
public:
	BMPFormatter(Composition*, int);

	void readColorMap(); // Loads in the color map
	int getValue(string, int);
	void pushColors(int, int, int);
	void addColors(int, string);
	uint8_t getColorMix(vector<uint8_t>&);
	void generateBoilerplate(ofstream&, uint32_t, uint32_t, uint32_t);

	void format() override;
};