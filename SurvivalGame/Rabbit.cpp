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
	// 새끼를 낳을 수 있는 공간 확인
	bool breedableCell[4] =
	{
		mWorld.IsValid(mX, mY - 1) && mWorld.CanBreed(mX, mY - 1),
		mWorld.IsValid(mX, mY + 1) && mWorld.CanBreed(mX, mY + 1),
		mWorld.IsValid(mX + 1, mY) && mWorld.CanBreed(mX + 1, mY),
		mWorld.IsValid(mX - 1, mY) && mWorld.CanBreed(mX - 1, mY)
	};

	// 새끼를 낳을 수 있는 공간 개수 확인
	int breedableCellCount = 0;
	for (int i = 0; i < 4; ++i)
	{
		if (breedableCell[i])
		{
			++breedableCellCount;
		}
	}

	// 공간이 없으면 생략
	if (breedableCellCount == 0)
	{
		return false;
	}

	// 새끼를 낳을 수 있는 공간 중 랜덤하게 선택
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

	// 새로 태어난 개체는 움직이지 않도록 설정.
	child->mIsActioned = true;
	
	mWorld.AddPiece(child);	
	return true;
}


void Rabbit::Move()
{
	mIsActioned = true;
	
	// 움직일 수 있는 셀 확인
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
	
	// 풀이 있는 셀 확인
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
		// 풀이 있는 위치 중 랜덤하게 선택
		direction = rand() % grassCellCount;
		while (grassCell[direction] == false)
		{
			direction = (direction + 1) % 4;
		}
	}
	else
	{
		// 움직 일 수 있는 위치 중 랜덤하게 선택
		direction = rand() % moveableCellCount;
		while (moveableCell[direction] == false || grassCell[direction] == true)
		{
			direction = (direction + 1) % 4;
		}
	}

	int newX = mX;
	int newY = mY;

	// 선택된 방향쪽으로 새로운 좌표 대입
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

	// 기존에 겹쳐있는 Piece를 저장 
	Piece* alreadyObscured = mObscured;
	mObscured = nullptr;
	
	// Grass가 있다면 제거 후 life를 최대치로
	if (mWorld.HasGrass(newX, newY))
	{
		Piece* grass = mWorld.RemovePiece(newX, newY);
		delete grass;

		mLife = 5;
	}

	// 음식이 있다면 저장 후 World에서 임시 제거
	else if (mWorld.HasFood(newX, newY))
	{
		mObscured = mWorld.RemovePiece(newX, newY);
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

