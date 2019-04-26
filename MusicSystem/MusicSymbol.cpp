#include "MusicSymbol.h"

MusicSymbol::MusicSymbol(Duration d) : d(d), isSplit(false) {}

Duration MusicSymbol::getDuration() const {
	return d;
}

ostream& operator<<(ostream& os, MusicSymbol& m) {
	m.getInfo(os);
	return os;
}

void MusicSymbol::getInfo(ostream& os) {
	os << "";
}