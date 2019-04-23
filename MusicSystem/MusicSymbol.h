#pragma once
#include "Duration.h"

class MusicSymbol {
	Duration d;
	bool isSplit;
public:
	MusicSymbol(Duration d);

	Duration getDuration() const;
};