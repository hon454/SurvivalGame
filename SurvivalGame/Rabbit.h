#pragma once
#include "Critter.h"
#include "Food.h"

class Rabbit : public Critter
{
public:
	Rabbit(World& w, int x, int y) : Critter(w, x, y, 'R', 5) { }	
	bool Breed();
	void Move() override;
private:
};
