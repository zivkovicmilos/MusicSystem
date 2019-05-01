#pragma once
#include "Formatter.h"

class MXMLFormatter : public Formatter {
private:
	void printNote(ostream&, MusicSymbol*, bool&);
	void additionalNotes(ostream&, Note*, int);
public:
	MXMLFormatter(Composition*);

	void format() override;
};