#pragma once
#include "Formatter.h";
#include <tuple>
#include <regex>

class BMPFormatter : Formatter {
	map<string, tuple<int, int, int>>* colorMap;
	vector<int> red;
	vector<int> green;
	vector<int> blue;
public:
	BMPFormatter(Composition*);

	void readColorMap();
	int getValue(string, int);
	void pushColors(int, int, int);
	void addColors(int, string);

	void format() override;
};