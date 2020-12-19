#include <cstdlib>
#include <cassert>

#include "Critter.h"
#include "World.h"

void Critter::Move(int direction)
{    
    int newX = mX;
    int newY = mY;

    // 0 : ¡®Up¡¯, 1: ¡®Down¡¯, 2 : ¡®Right¡¯, 3 : ¡®Left
    switch (direction)
    {
    case 0:
        --newY;
        break;
    case 1:
        ++newY;
        break;
    case 2:
        ++newX;
        break;
    case 3:
        --newX;
        break;
    default:
        assert("Not Valid Direction");
    }

    if (mWorld.IsValid(newX, newY) && mWorld.IsEmpty(newX, newY))
    {    	
        mWorld.MovePiece(mX, mY, newX, newY);
        mX = newX;
        mY = newY;
    }
}

void Critter::Move()
{
	const int direction = rand() % 4;
    Move(direction);
}
