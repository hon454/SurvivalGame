#include "Hunter.h"

#include <cassert>
#include <cstdlib>

#include "Food.h"
#include "World.h"

void Hunter::Move(int direction)
{
	mIsActioned = true;

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

	if (!mWorld.IsValid(newX, newY) || mWorld.HasTiger(newX, newY) || mWorld.HasRabbit(newX, newY))
	{
		return;
	}
	
	// temporary save obscured
	Piece* alreadyObscured = mObscured;
	mObscured = nullptr;

	// if grass on destination, remove grass and save
	if (mWorld.HasGrass(newX, newY))
	{
		mObscured = mWorld.RemovePiece(newX, newY);
	}
	// if food on destination, remove and life to full
	else if (mWorld.HasFood(newX, newY))
	{
		Piece* food = mWorld.RemovePiece(newX, newY);
		delete food;

		mLife = 8;
	}

	// move to destination
	mWorld.MovePiece(mX, mY, newX, newY);

	// if already have obscured, restore piece on source
	if (alreadyObscured != nullptr)
	{
		mWorld.AddPiece(alreadyObscured);
	}

	mX = newX;
	mY = newY;
}

void Hunter::Shoot(int direction)
{
	int deltaX = 0;
	int deltaY = 0;

	// 0 : ¡®Up¡¯, 1: ¡®Down¡¯, 2 : ¡®Right¡¯, 3 : ¡®Left
	// set delta value by direction
	switch (direction)
	{
	case 0:
		deltaY = -1;
		break;
	case 1:
		deltaY = 1;
		break;
	case 2:
		deltaX = 1;
		break;
	case 3:
		deltaX = -1;
		break;
	default:
		assert("Not Valid Direction");
	}
	
	int targetX = mX + deltaX;
	int targetY = mY + deltaY;

	// repeat until target position is valid 
	while(mWorld.IsValid(targetX, targetY))
	{
		// if animal is on destination, kill and generate food
		if (mWorld.HasRabbit(targetX, targetY) || mWorld.HasTiger(targetX, targetY))
		{
			Critter* killed = dynamic_cast<Critter*>(mWorld.RemovePiece(targetX, targetY));	
			Piece* obscured = killed->GetObscured();
			delete obscured;
			delete killed;
			mWorld.AddPiece(new Food(mWorld, targetX, targetY));
			break;
		}

		targetX += deltaX;
		targetY += deltaY;
	}
}
