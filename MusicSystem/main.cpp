#include <iostream>
#include <map>
#include <iterator>
#include <utility>
#include <regex>
#include <fstream>

using namespace std;

int main() {
	ifstream file;
	file.open("C:\\Users\\Milos\\Desktop\\FAKS\\POOP\\Projekat 1\\MusicSystem\\map.csv");
	string textLine;
	regex rx("([^,]*),([^,]*),([^,]*)*");
	map<char, pair<string, int>> noteMap;

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
	
	ifstream file2;
	file2.open("C:\\Users\\Milos\\Desktop\\FAKS\\POOP\\Projekat 1\\MusicSystem\\input\\ode_to_joy.txt");
	regex rx("([^,]*),([^,]*),([^,]*)*");

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

	return 0;
}