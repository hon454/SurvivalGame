#include <cassert>
#include <cstdlib>

#include "Tiger.h"
#include "World.h"

bool Tiger::Breed()
{
	mIsActioned = true;

	// check every 10 steps
	if (mAliveStep == 0 || mAliveStep % 10 != 0)
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

	// ���� �¾ ��ü�� �������� �ʵ��� ����.
	child->mIsActioned = true;

	mWorld.AddPiece(child);
	return true;
}

void Tiger::Move()
{
	mIsActioned = true;

	// 0 : ��Up��, 1: ��Down��, 2 : ��Right��, 3 : ��Left
	bool moveableCell[4] =
	{
		mWorld.IsValid(mX, mY - 1) && !mWorld.HasTiger(mX, mY - 1),
		mWorld.IsValid(mX, mY + 1) && !mWorld.HasTiger(mX, mY + 1),
		mWorld.IsValid(mX + 1, mY) && !mWorld.HasTiger(mX + 1, mY),
		mWorld.IsValid(mX - 1, mY) && !mWorld.HasTiger(mX - 1, mY)
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

	bool hunterCell[4] =
	{
		moveableCell[0] && mWorld.HasHunter(mX, mY - 1),
		moveableCell[1] && mWorld.HasHunter(mX, mY + 1),
		moveableCell[2] && mWorld.HasHunter(mX + 1, mY),
		moveableCell[3] && mWorld.HasHunter(mX - 1, mY)
	};

	int hunterCellCount = 0;
	for (int i = 0; i < 4; ++i)
	{
		if (hunterCell[i])
		{
			++hunterCellCount;
		}
	}

	bool rabbitCell[4] =
	{
		moveableCell[0] && mWorld.HasRabbit(mX, mY - 1),
		moveableCell[1] && mWorld.HasRabbit(mX, mY + 1),
		moveableCell[2] && mWorld.HasRabbit(mX + 1, mY),
		moveableCell[3] && mWorld.HasRabbit(mX - 1, mY)
	};

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
		// ��ɲ��� �ִ� ��ġ ����
		direction = rand() % hunterCellCount;
		while (!hunterCell[direction])
		{
			direction = (direction + 1) % 4;
		}
	}
	else if(rabbitCellCount > 0)
	{
		// �䳢�� �ִ� ��ġ �� �����ϰ� ����
		direction = rand() % rabbitCellCount;
		while (!rabbitCell[direction])
		{
			direction = (direction + 1) % 4;
		}
	}
	else
	{
		// ���� �� �� �ִ� ��ġ �� �����ϰ� ����
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

	// ������ �����ִ� Piece�� ���� 
	Piece* alreadyObscured = mObscured;
	mObscured = nullptr;

	// ��ɲ��� �ִٸ� ���� �� ��� ��Ŵ.
	if (mWorld.HasHunter(newX, newY))
	{
		mWorld.KillHunter();
	}
	
	// �䳢�� �ִٸ� ���� �� life�� �ִ�ġ��
	else if (mWorld.HasRabbit(newX, newY))
	{
		Piece* rabbit = mWorld.RemovePiece(newX, newY);
		delete rabbit;

		mLife = 7;
	}
	
	// �����̳� Grass�� �ִٸ� ���� �� World���� �ӽ� ����
	else if (mWorld.HasFood(newX, newY) || mWorld.HasGrass(newX, newY))
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
