#include "Hunter.h"

#include <cassert>
#include <cstdlib>

#include "World.h"

void Hunter::Move(int direction)
{
	mIsActioned = true;

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

	if (!mWorld.IsValid(newX, newY) || mWorld.HasTiger(newX, newY) || mWorld.HasRabbit(newX, newY))
	{
		return;
	}
	
	// ������ �����ִ� Piece�� ���� 
	Piece* alreadyObscured = mObscured;
	mObscured = nullptr;

	// Ǯ�� �ִٸ� ���� �� ����
	if (mWorld.HasGrass(newX, newY))
	{
		mObscured = mWorld.RemovePiece(newX, newY);
	}
	// ������ �ִٸ� ���� �� life�� �ִ�ġ��
	else if (mWorld.HasFood(newX, newY))
	{
		Piece* food = mWorld.RemovePiece(newX, newY);
		delete food;

		mLife = 7;
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

void Hunter::Shoot(int direction)
{
}
