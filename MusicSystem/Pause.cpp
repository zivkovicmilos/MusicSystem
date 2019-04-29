#include "Pause.h"

Pause::Pause(Duration d) : MusicSymbol(d) {
	isPause = true;
	isSpecial = false;
}

void Pause::splitDuration() {
	d.changeDuration(1, 8);
}

void Pause::setSpecial() {
	isSpecial = true;
}

bool Pause::checkSpecial() {
	return isSpecial;
}

void Pause::getInfo(ostream& os) {
	os << "P " << d;
}