#include <cassert>
#include <cstdlib>

#include "Tiger.h"
#include "World.h"

bool Tiger::Breed()
{
	mIsActioned = true;

	// check step
	if (mAliveStep == 0 || mAliveStep % 10 != 0)
	{
		return false;
	}

	// 0 : ‘Up’, 1: ‘Down’, 2 : ‘Right’, 3 : ‘Left
	// find breedable cells
	bool breedableCell[4] =
	{
		mWorld.IsValid(mX, mY - 1) && mWorld.CanBreed(mX, mY - 1),
		mWorld.IsValid(mX, mY + 1) && mWorld.CanBreed(mX, mY + 1),
		mWorld.IsValid(mX + 1, mY) && mWorld.CanBreed(mX + 1, mY),
		mWorld.IsValid(mX - 1, mY) && mWorld.CanBreed(mX - 1, mY)
	};

	// count breedable cells
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

	// select randomly in breedable cells
	int index = rand() % breedableCellCount;
	while (breedableCell[index] == false)
	{
		index = (index + 1) % 4;
	}

	// breed to breedable cell
	Tiger* child = nullptr;
	switch (index)
	{
	case 0:
		child = new Tiger(mWorld, mX, mY - 1);
		break;
	case 1:
		child = new Tiger(mWorld, mX, mY + 1);
		break;
	case 2:
		child = new Tiger(mWorld, mX + 1, mY);
		break;
	case 3:
		child = new Tiger(mWorld, mX - 1, mY);
		break;
	default:
		assert("Not Valid Index");
	}

	// child can not move step of birth
	child->mIsActioned = true;

	mWorld.AddPiece(child);
	return true;
}

void Tiger::Move()
{
	mIsActioned = true;

	// 0 : ‘Up’, 1: ‘Down’, 2 : ‘Right’, 3 : ‘Left
	bool moveableCell[4] =
	{
		mWorld.IsValid(mX, mY - 1) && !mWorld.HasTiger(mX, mY - 1),
		mWorld.IsValid(mX, mY + 1) && !mWorld.HasTiger(mX, mY + 1),
		mWorld.IsValid(mX + 1, mY) && !mWorld.HasTiger(mX + 1, mY),
		mWorld.IsValid(mX - 1, mY) && !mWorld.HasTiger(mX - 1, mY)
	};

	// 움직일 수 있는 셀을 확인
	int moveableCellCount = 0;
	for (int i = 0; i < 4; ++i)
	{
		if (moveableCell[i])
		{
			++moveableCellCount;
		}
	}

	if (moveableCellCount == 0)
	{
		return;
	}

	// find hunter cell
	bool hunterCell[4] =
	{
		moveableCell[0] && mWorld.HasHunter(mX, mY - 1),
		moveableCell[1] && mWorld.HasHunter(mX, mY + 1),
		moveableCell[2] && mWorld.HasHunter(mX + 1, mY),
		moveableCell[3] && mWorld.HasHunter(mX - 1, mY)
	};

	// count hunter cell
	int hunterCellCount = 0;
	for (int i = 0; i < 4; ++i)
	{
		if (hunterCell[i])
		{
			++hunterCellCount;
		}
	}

	// find rabbit cell
	bool rabbitCell[4] =
	{
		moveableCell[0] && mWorld.HasRabbit(mX, mY - 1),
		moveableCell[1] && mWorld.HasRabbit(mX, mY + 1),
		moveableCell[2] && mWorld.HasRabbit(mX + 1, mY),
		moveableCell[3] && mWorld.HasRabbit(mX - 1, mY)
	};

	// count rabbit cell
	int rabbitCellCount = 0;
	for (int i = 0; i < 4; ++i)
	{
		if (rabbitCell[i])
		{
			++rabbitCellCount;
		}
	}

	int direction;
	if (hunterCellCount > 0)
	{
		// select direction of hunter
		direction = rand() % hunterCellCount;
		while (!hunterCell[direction])
		{
			direction = (direction + 1) % 4;
		}
	}
	else if(rabbitCellCount > 0)
	{
		// select direction of rabbit, randomly 
		direction = rand() % rabbitCellCount;
		while (!rabbitCell[direction])
		{
			direction = (direction + 1) % 4;
		}
	}
	else
	{
		// select direction of moveable(no hunter, no rabbit), randomly 
		direction = rand() % moveableCellCount;
		while (!moveableCell[direction] || hunterCell[direction] || rabbitCell[direction])
		{	
			direction = (direction + 1) % 4;
		}
	}

	int newX = mX;
	int newY = mY;

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

	// save temporary obscured
	Piece* alreadyObscured = mObscured;
	mObscured = nullptr;

	// if hunter is on destination, kill him
	if (mWorld.HasHunter(newX, newY))
	{
		mWorld.KillHunter();
	}
	
	// if rabbit is on destination, kill and life to full
	else if (mWorld.HasRabbit(newX, newY))
	{
		Critter* rabbit = dynamic_cast<Critter*>(mWorld.RemovePiece(newX, newY));
		Piece* obscured = rabbit->GetObscured();
		if (obscured != nullptr)
		{
			mWorld.AddPiece(obscured);
		}
		delete rabbit;

		mLife = 8;
	}
	
	// if food, grass on destination, remove and save
	else if (mWorld.HasFood(newX, newY) || mWorld.HasGrass(newX, newY))
	{
		mObscured = mWorld.RemovePiece(newX, newY);
	}

	mWorld.MovePiece(mX, mY, newX, newY);

	// restore already obscured
	if (alreadyObscured != nullptr)
	{
		mWorld.AddPiece(alreadyObscured);
	}

	mX = newX;
	mY = newY;
}
