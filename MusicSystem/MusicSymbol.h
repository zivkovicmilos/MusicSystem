#pragma once
#include "Duration.h"

class MusicSymbol {
protected:
	Duration d;
	int id = ++cnt; // used for iterating
	static int cnt;
	bool isPause;
	bool split;
public:
	MusicSymbol(Duration d);
	Duration getDuration() const;
	void setDuration(int, int);
	virtual void getInfo(ostream&);

	void virtual splitDuration() {}
	bool checkPause();
	void setSplit();
	void clearSplit();
	bool isSplit() const;
	int getID();
	
	friend ostream& operator<<(ostream&, MusicSymbol&);
};