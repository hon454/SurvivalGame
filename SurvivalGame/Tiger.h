#ifndef TIGER_H
#define TIGER_H

#include "Critter.h"

class Tiger : public Critter
{
public:
	Tiger(World& w, int x, int y) : Critter(w, x, y, 'T', 7) { }
	bool Breed();
	void Move();
private:
};

#endif
