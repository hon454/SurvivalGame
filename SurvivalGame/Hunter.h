#pragma once

#include "Critter.h"
#include "Grass.h"

class Hunter : public Critter
{
public:
	Hunter(World& w, int x, int y) : Critter(w, x, y, 'H', 7) {}
	Grass* GetObscuredGrass() const { return mObscuredGrass; }
private:
	Grass* mObscuredGrass = nullptr;
};
