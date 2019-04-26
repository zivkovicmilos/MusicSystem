#pragma once
#include "MusicSymbol.h"

class Pause : public MusicSymbol {
public:
	Pause(Duration);

	void getInfo(ostream&) override;
};