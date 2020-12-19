#pragma once
#include "Critter.h"
#include "Grass.h"

class Tiger : public Critter
{
public:
	Tiger(World& w, int x, int y) : Critter(w, x, y, 'T', 7) { }
	bool Bread();
	Grass* GetStandingGrass() const { return mStandingGrass; }
private:
	Grass* mStandingGrass;
};
