#include "Pause.h"

Pause::Pause(Duration d) : MusicSymbol(d) {
	isPause = true;
}

void Pause::splitDuration() {
	split = true;
	d.changeDuration(1, 8);
}

void Pause::getInfo(ostream& os) {
	bool upperCase = false;
	if (d == Duration(1, 4) && !isSplit()) {
		os << "P";
	}
	else {
		os << "p";
	}
}