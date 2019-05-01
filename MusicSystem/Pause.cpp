#include "Pause.h"

Pause::Pause(Duration d) : MusicSymbol(d) {
	isPause = true;
}

void Pause::splitDuration() {
	split = true;
	d.changeDuration(1, 8);
}

void Pause::setSplit() {
	split = true;
}

void Pause::getInfo(ostream& os) {
	os << "P " << d;
}