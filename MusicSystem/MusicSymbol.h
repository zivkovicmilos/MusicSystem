#pragma once
#include "Duration.h"

class MusicSymbol {
protected:
	Duration d;
	bool isSplit;
public:
	MusicSymbol(Duration d);
	Duration getDuration() const;
	virtual void getInfo(ostream&);
	
	friend ostream& operator<<(ostream&, MusicSymbol&);
};