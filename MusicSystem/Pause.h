#pragma once
#include "MusicSymbol.h"

class Pause : public MusicSymbol {
public:
	Pause(Duration);

	//bool checkSpecial();
	
	void getInfo(ostream&) override;
	void splitDuration() override;
};