#include "MXMLFormatter.h"

using namespace std;

MXMLFormatter::MXMLFormatter(Composition* comp) : Formatter(comp) {}

void MXMLFormatter::additionalNotes(ostream& os, Note* n, int duration) {
	os << "<note>" << endl;
	os << "<chord/>" << endl;
	os << "<pitch>" << endl;
	os << "<step>" << n->getPitchC() << "</step>" << endl;
	os << "<octave>" << n->getOctave() << "</octave>" << endl;
	if (n->checkSharp()) {
		os << "<alter>1</alter>" << endl;
	}
	os << "</pitch>" << endl;
	os << "<duration>" << duration << "</duration>" << endl;
	os << "</note>" << endl;
}

void MXMLFormatter::printNote(ostream& os, MusicSymbol* m, bool& split) {

	int duration = m->getDuration().getDenominator() == 4 ? 2 : 1;
	os << "<note>" << endl;
	if (m->checkPause()) {
		Pause* p = (Pause*)m;
		os << "<rest/>" << endl;
		os << "<duration>" << duration << "</duration>" << endl;
		os << "</note>" << endl;
	}
	else {
		Note* n = (Note*)m;
		os << "<pitch>" << endl;
		os << "<step>" << n->getPitchC() << "</step>" << endl;
		os << "<octave>" << n->getOctave() << "</octave>" << endl;
		if (n->checkSharp()) {
			os << "<alter>1</alter>" << endl;
		}
		os << "</pitch>" << endl;
		os << "<duration>" << duration << "</duration>" << endl;
		if (n->checkSplit() && !split) {
			os << "<tie type=\"start\"/>" << endl;
			split = true;
		}
		else if (split) {
			os << "<tie type=\"end\"/>" << endl;
			split = false;
		}
		os << "</note>" << endl;

		if (n->getNext()) {
			Note* temp = n->getNext();
			while (temp) {
				additionalNotes(os, temp, duration);
				temp = temp->getNext();
			}
		}
	}
}

void MXMLFormatter::format() {
	ifstream reader;
	ofstream output;
	string outputFileName;
	cout << "Enter the MXML output file name: ";
	cout << "> ";
	cin >> outputFileName;

	bool openMeasure = true;

	reader.open("MusicXML\\start.txt");
	output.open("MusicXML\\"+outputFileName+".musicxml");

	string textLine;
	while (getline(reader, textLine)) {
		output << textLine;
	}
	reader.close();
	reader.open("MusicXML\\rightBP.txt");

	output << "<beats>" << comp->getDuration().getNumerator() << "</beats>" << endl;
	output << "<beat-type>" << comp->getDuration().getDenominator() << "</beat-type>" << endl;

	while (getline(reader, textLine)) {
		output << textLine;
	}
	reader.close();
	// Right boilerplate done
	vector<Measure*>* measureArr = comp->getMeasureArr();
	vector<MusicSymbol*>* left = nullptr;
	vector<MusicSymbol*>* right = nullptr;
	bool split = false;

	for (int i = 0; i < measureArr->size(); i++) {
		right = (*measureArr)[i]->getRight();
		if (!openMeasure) {
			output << "<measure>" << endl;
		}
		for (int j = 0; j < right->size(); j++) {
			printNote(output, (*right)[j], split);
		}
		output << "</measure>" << endl;
		openMeasure = false;
	}
	output << "</part>" << endl;
	// Right part done

	output << "<part id=\"Left\">" << endl;
	output << endl;
	output << "<measure>" << endl << "<attributes>" << endl;
	output << "<divisions>2</divisions>" << endl;
	output << "<time>" << endl;
	output << "<beats>" << comp->getDuration().getNumerator() << "</beats>" << endl;
	output << "<beat-type>" << comp->getDuration().getDenominator() << "</beat-type>" << endl;
	output << "</time>" << endl << "<clef>" << endl << "<sign>F</sign>" << endl << "<line>4</line>" << endl;
	output << "</clef>" << endl;
	output << "</attributes>" << endl;
	openMeasure = true;
	split = false;

	for (int i = 0; i < measureArr->size(); i++) {
		left = (*measureArr)[i]->getLeft();
		if (!openMeasure) {
			output << "<measure>" << endl;
		}
		for (int j = 0; j < left->size(); j++) {
			printNote(output, (*left)[j], split);
		}
		output << "</measure>" << endl;
		openMeasure = false;
	}
	output << "</part>" << endl;
	output << "</score-partwise>";
	output.close();
}