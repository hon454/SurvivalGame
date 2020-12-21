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

	// 0 : ��Up��, 1: ��Down��, 2 : ��Right��, 3 : ��Left
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

	// ���� �¾ ��ü�� �������� �ʵ��� ����.
	child->mIsActioned = true;
	
	mWorld.AddPiece(child);	
	return true;
}


void Rabbit::Move()
{
	mIsActioned = true;
	
	// 0 : ��Up��, 1: ��Down��, 2 : ��Right��, 3 : ��Left
	bool moveableCell[4] =
	{
		mWorld.IsValid(mX, mY - 1) && (mWorld.IsEmpty(mX, mY - 1) || mWorld.HasFood(mX, mY - 1) || mWorld.HasGrass(mX, mY - 1)),
		mWorld.IsValid(mX, mY + 1) && (mWorld.IsEmpty(mX, mY + 1) || mWorld.HasFood(mX, mY + 1) || mWorld.HasGrass(mX, mY + 1)),
		mWorld.IsValid(mX + 1, mY) && (mWorld.IsEmpty(mX + 1, mY) || mWorld.HasFood(mX + 1, mY) || mWorld.HasGrass(mX + 1, mY)),
		mWorld.IsValid(mX - 1, mY) && (mWorld.IsEmpty(mX - 1, mY) || mWorld.HasFood(mX - 1, mY) || mWorld.HasGrass(mX - 1, mY))
	};

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

	bool grassCell[4] =
	{
		moveableCell[0] && mWorld.HasGrass(mX, mY - 1),
		moveableCell[1] && mWorld.HasGrass(mX, mY + 1),
		moveableCell[2] && mWorld.HasGrass(mX + 1, mY),
		moveableCell[3] && mWorld.HasGrass(mX - 1, mY)
	};

	int grassCellCount = 0;
	for (int i = 0; i < 4; ++i)
	{
		if (grassCell[i])
		{
			++grassCellCount;
		}
	}

	int direction;
	if (grassCellCount > 0 )
	{
		// Ǯ�� �ִ� ��ġ �� �����ϰ� ����
		direction = rand() % grassCellCount;
		while (grassCell[direction] == false)
		{
			direction = (direction + 1) % 4;
		}
	}
	else
	{
		// ���� �� �� �ִ� ��ġ �� �����ϰ� ����
		direction = rand() % moveableCellCount;
		while (moveableCell[direction] == false || grassCell[direction] == true)
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

	// ������ �����ִ� Piece�� ���� 
	Piece* alreadyObscured = mObscured;
	mObscured = nullptr;
	
	// Grass�� �ִٸ� ���� �� life�� �ִ�ġ��
	if (mWorld.HasGrass(newX, newY))
	{
		Piece* grass = mWorld.RemovePiece(newX, newY);
		delete grass;

		mLife = 5;
	}

	// ������ �ִٸ� ���� �� World���� �ӽ� ����
	else if (mWorld.HasFood(newX, newY))
	{
		mObscured = mWorld.RemovePiece(newX, newY);
	}
	
	mWorld.MovePiece(mX, mY, newX, newY);

	// ������ �����ִ� Piece�� �̵� �� ��ġ�� ����
	if (alreadyObscured != nullptr)
	{
		mWorld.AddPiece(alreadyObscured);
	}
	
	mX = newX;
	mY = newY;
}

