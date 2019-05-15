#pragma once
#include <iterator>
#include <iostream>
#include "Composition.h"
#include "MXMLFormatter.h"
#include "MIDIFormatter.h"
#include "BMPFormatter.h"
using namespace std;

class Menu {
public:
	bool changed = false;
	void mainMenu();
	void compIterMenu();
	void noteIterMenu(bool);
	void chordIterMenu();
	void compositionText();
	void exportText();
	void exportMenu(Composition*, map<string, int>&,
		MXMLFormatter*, MIDIFormatter*, BMPFormatter*);
	void compositionMenu(Composition*);
};