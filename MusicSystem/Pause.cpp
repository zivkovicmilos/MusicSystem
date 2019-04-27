#include "Pause.h"

Pause::Pause(Duration d) : MusicSymbol(d) {
	isPause = true;
}

void Pause::getInfo(ostream& os) {
	os << "P " << d;
}