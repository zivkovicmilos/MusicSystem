#include "MusicSymbol.h"

MusicSymbol::MusicSymbol(Duration d) : d(d), isSplit(false) {}

Duration MusicSymbol::getDuration() const {
	return d;
}