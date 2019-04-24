#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <map>
#include <iterator>
#include <utility>
#include <regex>
#include <fstream>
#include "Duration.h"
#include "MusicSymbol.h"
#include "vector"

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

void addSymbols(vector<Temp*>* noteMap, regex rx, string str) {
	smatch match;
	bool first = true;
	int num = 0;
	int oldLength = 0;
	//string realStr = str;
	string::const_iterator searchStart(str.cbegin());
	//regex outside("([a-zA-Z0-9]{1,}(?![^\\[\\]]*\\]))");
	while (regex_search(searchStart, str.cend(), match, rx)) {
		//cout << (searchStart == textLine.cbegin() ? "" : " ") << match[0] << endl;
		if (first) {
			num = 0;
			first = false;
		}
		else {
			int np = match.position();
			int ln = match.str().length();
			num += oldLength + match.position();
		}
		cout << match.str() << " at " << num << endl;
		oldLength = match.str().length();
		//"at pos " << match.position() 
		int n = match.str().length();
		if (n>1) {
			char* charArr = new char[n + 1];
			strcpy(charArr, match.str().c_str());
			int offset = 0;
			for (int i = 0; i < n; i++) {
				Temp* temp = new Temp(charArr[i],num+offset,Duration(1,4));
				offset++;
				noteMap->push_back(temp);
			}
		}
		else {
			Temp* temp = new Temp(match.str()[0], num, Duration(1, 4));
			noteMap->push_back(temp);
		}
		searchStart = match.suffix().first;
	}
}

int main() {
	ifstream file;
	file.open("C:\\Users\\Milos\\Desktop\\FAKS\\POOP\\Projekat 1\\MusicSystem\\map.csv");
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
			//cout << character << " => " << note << ", " << midiNum << endl;
		}
	}

	file.close();
	ifstream file2;
	//file2.open("‪C:\\Users\\Milos\\Desktop\\test.txt");

	file2.open("test.txt");
	regex outside("([a-zA-Z0-9]{1,}(?![^\\[\\]]*\\]))");

	while (getline(file2, textLine)) {
		// For now just the whole words / characters
		// TODO: Add support for spaces + between brackets, SORT
		addSymbols(&allSymbols, outside, textLine);
		cout << endl;
		cout << "PRINTING FROM ARRAY:" << endl;
		for (int i = 0; i < allSymbols.size(); i++) {
			cout << allSymbols[i]->note << " at index: " << allSymbols[i]->indx << endl;
		}
		
		cout << endl;
	}
	return 0;
}