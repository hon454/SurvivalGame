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
	
	// 기존에 겹쳐있는 Piece를 저장 
	Piece* alreadyObscured = mObscured;
	mObscured = nullptr;

	// 풀이 있다면 제거 후 저장
	if (mWorld.HasGrass(newX, newY))
	{
		mObscured = mWorld.RemovePiece(newX, newY);
	}
	// 음식이 있다면 제거 후 life를 최대치로
	else if (mWorld.HasFood(newX, newY))
	{
		Piece* food = mWorld.RemovePiece(newX, newY);
		delete food;

		mLife = 7;
	}

	mWorld.MovePiece(mX, mY, newX, newY);

	// 기존에 겹쳐있던 Piece를 이동 전 위치에 복원
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
