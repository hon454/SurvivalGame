#ifndef RABBIT_H
#define RABBIT_H

#include "Critter.h"
#include "Food.h"

class Rabbit : public Critter
{
public:
	Rabbit(World& w, int x, int y) : Critter(w, x, y, 'R', 5) { }	
	bool Breed();
	void Move();
private:
};

#endif

