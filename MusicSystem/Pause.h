#pragma once
#include "MusicSymbol.h"

class Pause : public MusicSymbol {
	bool isSpecial;
public:
	Pause(Duration);

	bool checkSpecial();
	void setSpecial();
	void getInfo(ostream&) override;
	void splitDuration() override;
};