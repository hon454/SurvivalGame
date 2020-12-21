#include <cstdlib>
#include <cassert>

#include "Rabbit.h"
#include "World.h"

bool Rabbit::Breed()
{
	mIsActioned = true;
	
	if (mAliveStep == 0 || mAliveStep % 7 != 0)
	{
		return false;
	}

	// 0 : ‘Up’, 1: ‘Down’, 2 : ‘Right’, 3 : ‘Left
	bool adjacentCell[4] =
	{
		mWorld.IsValid(mX, mY - 1) && mWorld.CanBreed(mX, mY - 1),
		mWorld.IsValid(mX, mY + 1) && mWorld.CanBreed(mX, mY + 1),
		mWorld.IsValid(mX + 1, mY) && mWorld.CanBreed(mX + 1, mY),
		mWorld.IsValid(mX - 1, mY) && mWorld.CanBreed(mX - 1, mY)
	};

	int breedableCellCount = 0;
	for (int i = 0; i < 4; ++i)
	{
		if (adjacentCell[i])
		{
			++breedableCellCount;
		}
	}

	if (breedableCellCount == 0)
	{
		return false;
	}

	int index = rand() % breedableCellCount;
	while (adjacentCell[index] == false)
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

	// 새로 태어난 개체는 움직이지 않도록 설정.
	child->mIsActioned = true;
	
	mWorld.AddPiece(child);	
	return true;
}


void Rabbit::Move()
{
	mIsActioned = true;
	
	// 0 : ‘Up’, 1: ‘Down’, 2 : ‘Right’, 3 : ‘Left
	bool adjacentCell[4] =
	{
		mWorld.IsValid(mX, mY - 1) && (mWorld.IsEmpty(mX, mY - 1) || mWorld.HasFood(mX, mY - 1) || mWorld.HasGrass(mX, mY - 1)),
		mWorld.IsValid(mX, mY + 1) && (mWorld.IsEmpty(mX, mY + 1) || mWorld.HasFood(mX, mY + 1) || mWorld.HasGrass(mX, mY + 1)),
		mWorld.IsValid(mX + 1, mY) && (mWorld.IsEmpty(mX + 1, mY) || mWorld.HasFood(mX + 1, mY) || mWorld.HasGrass(mX + 1, mY)),
		mWorld.IsValid(mX - 1, mY) && (mWorld.IsEmpty(mX - 1, mY) || mWorld.HasFood(mX - 1, mY) || mWorld.HasGrass(mX - 1, mY))
	};

	int moveableCellCount = 0;
	for (int i = 0; i < 4; ++i)
	{
		if (adjacentCell[i])
		{
			++moveableCellCount;
		}
	}

	if (moveableCellCount == 0)
	{
		return;
	}

	int direction = rand() % moveableCellCount;
	while (adjacentCell[direction] == false)
	{
		direction = (direction + 1) % 4;
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

	// Grass가 있다면 제거 후 life를 최대치로
	if (mWorld.HasGrass(newX, newY))
	{
		Piece* grass = mWorld.RemovePiece(newX, newY);
		delete grass;

		mLife = 5;
	}

	// 음식이 있다면 저장 후 World에서 임시 제거
	if (mWorld.HasFood(newX, newY))
	{
		mObscuredFood = mWorld.RemovePiece(newX, newY);
	}
	
	mWorld.MovePiece(mX, mY, newX, newY);
	mX = newX;
	mY = newY;
}

