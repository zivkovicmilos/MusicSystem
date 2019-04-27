#pragma once
#include "Duration.h"

class MusicSymbol {
protected:
	Duration d;
	bool isPause;
public:
	MusicSymbol(Duration d);
	Duration getDuration() const;
	void setDuration(int, int);
	virtual void getInfo(ostream&);

	bool checkPause();
	
	friend ostream& operator<<(ostream&, MusicSymbol&);
};