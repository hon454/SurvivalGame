#pragma once

#include "Critter.h"

class Hunter : public Critter
{
public:
	Hunter(World& w, int x, int y) : Critter(w, x, y, 'H', 7) {}
private:
};