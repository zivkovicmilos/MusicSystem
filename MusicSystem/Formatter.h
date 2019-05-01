#pragma once
#include "Composition.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>

class Formatter {
protected:
	Composition* comp;
public:
	Formatter(Composition*);

	virtual void format() = 0;
};