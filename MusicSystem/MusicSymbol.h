#pragma once
#include "Duration.h"

class MusicSymbol {
protected:
	Duration d;
	bool isPause;
	bool split;
public:
	MusicSymbol(Duration d);
	Duration getDuration() const;
	void setDuration(int, int);
	virtual void getInfo(ostream&);

	void virtual splitDuration() {}
	bool checkPause();
	bool isSplit();
	
	friend ostream& operator<<(ostream&, MusicSymbol&);
};