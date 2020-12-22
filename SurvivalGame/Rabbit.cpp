#include <cstdlib>
#include <cassert>

#include "Rabbit.h"
#include "World.h"

bool Rabbit::Breed()
{
	mIsActioned = true;

	// check step
	if (mAliveStep == 0 || mAliveStep % 7 != 0)
	{
		return false;
	}

	// 0 : ¡®Up¡¯, 1: ¡®Down¡¯, 2 : ¡®Right¡¯, 3 : ¡®Left
	// find breedable cells
	bool breedableCell[4] =
	{
		mWorld.IsValid(mX, mY - 1) && mWorld.CanBreed(mX, mY - 1),
		mWorld.IsValid(mX, mY + 1) && mWorld.CanBreed(mX, mY + 1),
		mWorld.IsValid(mX + 1, mY) && mWorld.CanBreed(mX + 1, mY),
		mWorld.IsValid(mX - 1, mY) && mWorld.CanBreed(mX - 1, mY)
	};

	// count breedable cell
	int breedableCellCount = 0;
	for (int i = 0; i < 4; ++i)
	{
		if (breedableCell[i])
		{
			++breedableCellCount;
		}
	}

	// if no breedable cells, skip
	if (breedableCellCount == 0)
	{
		return false;
	}

	// randomly select in breedable cells
	int index = rand() % breedableCellCount;
	while (breedableCell[index] == false)
	{
		index = (index + 1) % 4;
	}

	Rabbit* child = nullptr;
	switch (index)
	{
	case 0:
		child = new Rabbit(mWorld, mX, mY - 1);
		break;
	case 1:
		child = new Rabbit(mWorld, mX, mY + 1);
		break;
	case 2:
		child = new Rabbit(mWorld, mX + 1, mY);
		break;
	case 3:
		child = new Rabbit(mWorld, mX - 1, mY);
		break;
	default: 
		assert("Not Valid Index");
	}

	// child can not action in currently step
	child->mIsActioned = true;
	
	mWorld.AddPiece(child);	
	return true;
}


void Rabbit::Move()
{
	mIsActioned = true;
	
	// find moveable cells
	bool moveableCell[4] =
	{
		mWorld.IsValid(mX, mY - 1) && (mWorld.IsEmpty(mX, mY - 1) || mWorld.HasFood(mX, mY - 1) || mWorld.HasGrass(mX, mY - 1)),
		mWorld.IsValid(mX, mY + 1) && (mWorld.IsEmpty(mX, mY + 1) || mWorld.HasFood(mX, mY + 1) || mWorld.HasGrass(mX, mY + 1)),
		mWorld.IsValid(mX + 1, mY) && (mWorld.IsEmpty(mX + 1, mY) || mWorld.HasFood(mX + 1, mY) || mWorld.HasGrass(mX + 1, mY)),
		mWorld.IsValid(mX - 1, mY) && (mWorld.IsEmpty(mX - 1, mY) || mWorld.HasFood(mX - 1, mY) || mWorld.HasGrass(mX - 1, mY))
	};

	// count movealbe cells
	int moveableCellCount = 0;
	for (int i = 0; i < 4; ++i)
	{
		if (moveableCell[i])
		{
			++moveableCellCount;
		}
	}

	// count moveable cells
	if (moveableCellCount == 0)
	{
		return;
	}
	
	// find grass cells
	bool grassCell[4] =
	{
		moveableCell[0] && mWorld.HasGrass(mX, mY - 1),
		moveableCell[1] && mWorld.HasGrass(mX, mY + 1),
		moveableCell[2] && mWorld.HasGrass(mX + 1, mY),
		moveableCell[3] && mWorld.HasGrass(mX - 1, mY)
	};

	// count grass cells
	int grassCellCount = 0;
	for (int i = 0; i < 4; ++i)
	{
		if (grassCell[i])
		{
			++grassCellCount;
		}
	}

	int direction;

	// if grass cell is exist
	if (grassCellCount > 0 )
	{
		// randomly select in grass cells
		direction = rand() % grassCellCount;
		while (grassCell[direction] == false)
		{
			direction = (direction + 1) % 4;
		}
	}
	// if grass cell is not exist
	else
	{
		// randomly select in moveable but not grass cells
		direction = rand() % moveableCellCount;
		while (moveableCell[direction] == false || grassCell[direction] == true)
		{
			direction = (direction + 1) % 4;
		}
	}

	int newX = mX;
	int newY = mY;

	// 0 : ¡®Up¡¯, 1: ¡®Down¡¯, 2 : ¡®Right¡¯, 3 : ¡®Left
	// set destination from direction
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

	// remove and temporary save obscured
	Piece* alreadyObscured = mObscured;
	mObscured = nullptr;
	
	// if grass on destination, life to full and remove grass
	if (mWorld.HasGrass(newX, newY))
	{
		Piece* grass = mWorld.RemovePiece(newX, newY);
		delete grass;

		mLife = 6;
	}

	// if food on destination, remove and save
	else if (mWorld.HasFood(newX, newY))
	{
		mObscured = mWorld.RemovePiece(newX, newY);
	}

	// move to destination
	mWorld.MovePiece(mX, mY, newX, newY);

	// restore already obscured piece
	if (alreadyObscured != nullptr)
	{
		mWorld.AddPiece(alreadyObscured);
	}
	
	mX = newX;
	mY = newY;
}

