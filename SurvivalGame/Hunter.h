#pragma once

#include "Critter.h"

class Hunter : public Critter
{
public:
	Hunter(World& world, int x, int y, int life);
	void Move(int direction);
	void Shoot(int direction);
private:
	int mLife;
	World& mWorld;
};