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

	// 0 : ‘Up’, 1: ‘Down’, 2 : ‘Right’, 3 : ‘Left
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

		mLife = 8;
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
	int deltaX = 0;
	int deltaY = 0;

	// 0 : ‘Up’, 1: ‘Down’, 2 : ‘Right’, 3 : ‘Left
	// 방향에 따라 추가 되는 delta 값을 설정
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

	// 타겟의 위치가 그리드 밖으로 나갈 때 까지 반복
	while(mWorld.IsValid(targetX, targetY))
	{
		// 만약 동물이라면 죽이고 음식을 생성
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
