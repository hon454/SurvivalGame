#include <iostream>

#include "World.h"
#include "Rabbit.h"
#include "Tiger.h"
#include "Hunter.h"
#include "Grass.h"
#include "Food.h"

using namespace std;

World::World(int H, int W, int G, int R, int T)
	:mHeight(H), mWidth(W), mTimeStep(1)
{
	// 그리드 동적 배열 구성 및 
	mGrid = new Piece**[mHeight];	
	for (int i = 0; i < mHeight; ++i)
	{
		mGrid[i] = new Piece*[mWidth];
		for (int j = 0; j < mWidth; ++j)
		{
			mGrid[i][j] = nullptr;
		}
	}

	int x, y;

	// 사냥꾼 생성
	getEmptyCell(x, y);
	mHunter = new Hunter(*this, x, y);
	AddPiece(mHunter);

	// 토끼 생성
	for (int i = 0; i < R; ++i)
	{
		getEmptyCell(x, y);
		AddPiece(new Rabbit(*this, x, y));
	}

	// 호랑이 생성
	for (int i = 0; i < T; ++i)
	{
		getEmptyCell(x, y);
		AddPiece(new Tiger(*this, x, y));
	}

	// 풀 생성
	for (int i = 0; i < G; ++i)
	{
		getEmptyCell(x, y);
		AddPiece(new Grass(*this, x, y));
	}
}

World::~World()
{
	// 메모리 해제
    for (int i = 0; i < mHeight; ++i)
    {
        for (int j = 0; j < mWidth; ++j)
        {       	
			delete mGrid[i][j];
        }
        delete[] mGrid[i];
    }
    delete[] mGrid;
}

bool World::IsValid(int x, int y) const
{
	return x >= 0 && x < mWidth&& y >= 0 && y < mHeight;
}

bool World::IsEmpty(int x, int y) const
{
	return mGrid[y][x] == nullptr;
}

bool World::IsGameWin() const
{
	// 호랑이가 남아있는지 검색
	for (int i = 0; i < mHeight; ++i)
	{
		for (int j = 0; j < mWidth; ++j)
		{
			if (dynamic_cast<Tiger*>(mGrid[i][j]) != nullptr)
			{
				return false;
			}
		}
	}

	cout << "Tigers are all dead" << endl;
	return true;
}

bool World::IsGameOver() const
{
	if (mHunter == nullptr)
	{
		if (mIsHunterStarved)
		{
			cout << "Hunter starved to death" << endl;
		}
		else
		{
			cout << "Hunter is dead" << endl;
		}
		return true;
	}
	
	for (int i = 0; i < mHeight; ++i)
	{
		for (int j = 0; j < mWidth; ++j)
		{
			if (dynamic_cast<Rabbit*>(mGrid[i][j]) != nullptr)
			{
				return false;
			}
		}
	}
	
	cout << "Rabbits are gone" << endl;
	return true;
}

bool World::CanBreed(int x, int y) const
{
	return IsEmpty(x, y) || HasFood(x, y) || HasGrass(x, y);
}

bool World::HasEmptyCell() const
{
	for (int i = 0; i < mHeight; ++i)
	{
		for (int j = 0; j < mWidth; ++j)
		{
			if (mGrid[i][j] == nullptr)
			{
				return true;
			}
		}
	}
	return false;
}

bool World::HasHunter(int x, int y) const
{
	if (IsEmpty(x, y))
	{
		return false;
	}

	return dynamic_cast<Hunter*>(mGrid[y][x]) != nullptr;
}

bool World::HasRabbit(int x, int y) const
{
	if (IsEmpty(x, y))
	{
		return false;
	}

	return dynamic_cast<Rabbit*>(mGrid[y][x]) != nullptr;
}

bool World::HasTiger(int x, int y) const
{
	if (IsEmpty(x, y))
	{
		return false;
	}

	return dynamic_cast<Tiger*>(mGrid[y][x]) != nullptr;
}

bool World::HasFood(int x, int y) const
{
	if (IsEmpty(x, y))
	{
		return false;
	}

	return dynamic_cast<Food*>(mGrid[y][x]) != nullptr;
}

bool World::HasGrass(int x, int y) const
{
	if (IsEmpty(x, y))
	{
		return false;
	}

	return dynamic_cast<Grass*>(mGrid[y][x]) != nullptr;
}

bool World::MovePiece(int x, int y, int newX, int newY)
{
	if (!IsValid(newX, newY) || !IsEmpty(newX, newY))
	{
		return false;
	}

	// 이동 후 기존 위치 초기화
	mGrid[newY][newX] = mGrid[y][x];
	mGrid[y][x] = nullptr;

	return true;
}

Piece* World::RemovePiece(int x, int y)
{
	Piece* removed = mGrid[y][x];
	mGrid[y][x] = nullptr;
	return removed;
}

bool World::AddPiece(Piece* piece)
{
	int x = piece->GetX();
	int y = piece->GetY();
	
	if (mGrid[y][x] != nullptr)
	{
		return false;
	}
	
	mGrid[y][x] = piece;
	return true;
}

void World::KillHunter()
{
	RemovePiece(mHunter->GetX(), mHunter->GetY());

	// 겹쳐있는 것이 있었다면 복원
	Piece* obscured = mHunter->GetObscured();
	if (obscured != nullptr)
	{
		AddPiece(obscured);
	}
	delete mHunter;
	mHunter = nullptr;
}

void World::Update(int command, int direction)
{
	// 사냥꾼, 호랑이, 토끼 순으로 업데이트 진행
	updateHunter(command, direction);
	updateTigers();
	updateRabbits();

	// 풀 생성
	generateGrass();

	// 생명체들의 life감소, aliveStep 증가, 행동 가능 초기화
	for (int i = 0; i < mHeight; ++i)
	{
		for (int j = 0; j < mWidth; ++j)
		{
			Critter* pCritter = dynamic_cast<Critter*>(mGrid[i][j]);			
			if (pCritter == nullptr)
			{
				continue;
			}

			pCritter->Alive();

			// 체력이 0이 된 Critter 들을 사망처리 한다.
			if (pCritter->GetLife() <= 0)
			{
				// 겹친 Piece가 있을 경우, 복원
				Piece* obscured = pCritter->GetObscured();
				if (obscured != nullptr)
				{
					mGrid[i][j] = obscured;					
				}
				else
				{
					RemovePiece(j, i);
				}

				// 이 생명체가 헌터인 경우 
				if (mHunter->GetLife() <= 0)
				{
					KillHunter();
					mIsHunterStarved = true;
				}
				else
				{
					delete pCritter;
				}
			}
		}
	}
	
	++mTimeStep;
}

void World::Display() const
{
	cout << endl;
	for (int i = 0; i < 50; ++i)
	{
		cout << "=";
	}
	cout << endl << endl;

	int hunterLife = 0;
	// 사냥꾼이 살아있는 경우에만 hunterLife를 갱신
	if (mHunter != nullptr)
	{
		hunterLife = mHunter->GetLife();
	}
	
	cout << "Time Step: " << mTimeStep << "\t\t" << "Life: " << hunterLife << endl;

	showGrid();

	cout << endl;
}

bool World::getEmptyCell(int& x, int& y) const
{	
	if (!HasEmptyCell())
	{
		return false;
	}		
	
	while (true)
	{
		x = rand() % mWidth;
		y = rand() % mHeight;
		if (IsEmpty(x, y))
		{
			break;
		}
	}
	return true;
}

void World::generateGrass()
{
	for (int i = 0; i < mHeight; ++i)
	{
		for (int j = 0; j < mWidth; ++j)
		{
			if (mGrid[i][j] == nullptr && rand() % 10 == 0)
			{
				mGrid[i][j] = new Grass(*this, j, i);
			}
		}
	}
}

void World::updateHunter(int command, int direction)
{	
	if (command == 0)
	{
		mHunter->Move(direction);
	}
	else if (command == 1)
	{
		mHunter->Shoot(direction);
	}
}

void World::updateTigers()
{
	for (int i = 0; i < mHeight; ++i)
	{
		for (int j = 0; j < mWidth; ++j)
		{
			Tiger* pTiger = dynamic_cast<Tiger*>(mGrid[i][j]);

			// 호랑이가 아닌 경우 생략
			if (pTiger == nullptr)
			{
				continue;
			}

			// 이미 활동 한 호랑이인 경우 생략
			if (pTiger->IsActioned())
			{
				continue;
			}

			if (pTiger->Breed())
			{
				continue;
			}

			pTiger->Move();
		}
	}
}

void World::updateRabbits()
{
	for (int i = 0; i < mHeight; ++i)
	{
		for (int j = 0; j < mWidth; ++j)
		{			
			Rabbit* pRabbit = dynamic_cast<Rabbit*>(mGrid[i][j]);

			// 토끼가 아닌 경우 생략
			if (pRabbit == nullptr)
			{
				continue;			
			}

			// 이미 활동 한 토끼인 경우 생략
			if (pRabbit->IsActioned())
			{
				continue;
			}

			if (pRabbit->Breed())
			{
				continue;
			}
			
			pRabbit->Move();
		}
	}
}

void World::showGrid() const
{
	char initial;
	for (int i = 0; i < mHeight; ++i)
	{
		for (int j = 0; j < mWidth; ++j)
		{
			if (mGrid[i][j] == nullptr)
			{
				initial = '-';
			}
			else
			{
				initial = mGrid[i][j]->GetInitial();
			}
			cout << initial << ' ';
		}
		cout << '\n';
	}
}
