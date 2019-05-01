#include "MusicSymbol.h"

MusicSymbol::MusicSymbol(Duration d) : d(d), isPause(false), split(false){}

Duration MusicSymbol::getDuration() const {
	return d;
}

void MusicSymbol::setDuration(int num, int denom) {
	d.changeDuration(num, denom);
}

bool MusicSymbol::isSplit() {
	return split;
}

ostream& operator<<(ostream& os, MusicSymbol& m) {
	m.getInfo(os);
	return os;
}

bool MusicSymbol::checkPause() {
	return isPause;
}

void MusicSymbol::getInfo(ostream& os) {
	os << "";
}