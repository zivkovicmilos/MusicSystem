#pragma once
#include "Formatter.h";
#include <tuple>
#include <regex>
#include <cstdint>

class BMPFormatter : Formatter {
	map<string, tuple<int, int, int>> colorMap;
	vector<int> red;
	vector<int> green;
	vector<int> blue;
	int width;
public:
	BMPFormatter(Composition*, int);

	void readColorMap();
	int getValue(string, int);
	void pushColors(int, int, int);
	void addColors(int, string);
	int getColorMix(vector<int>&);
	void generateBoilerplate(ofstream&, int, int, int);
	string numToHex(int num);

	void format() override;
};