#include "Pause.h"

Pause::Pause(Duration d) : MusicSymbol(d) {
	isSplit = false;
}

void Pause::getInfo(ostream& os) {
	os << "P " << d;
}