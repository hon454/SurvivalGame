#include <iostream>

#include "World.h"
#include "Rabbit.h"
#include "Tiger.h"
#include "Hunter.h"
#include "Grass.h"
#include "Food.h"

using namespace std;

World::World(int H, int W, int G, int R, int T)
	:mHeight(H), mWidth(W), mNumberOfTigers(T), mTimeStep(1)
{
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
	getEmptyCell(x, y);
	mHunter = new Hunter(*this, x, y);
	AddPiece(mHunter);
	
	for (int i = 0; i < R; ++i)
	{
		getEmptyCell(x, y);
		AddPiece(new Rabbit(*this, x, y));
	}

	for (int i = 0; i < T; ++i)
	{
		getEmptyCell(x, y);
		AddPiece(new Tiger(*this, x, y));
	}

	for (int i = 0; i < G; ++i)
	{
		getEmptyCell(x, y);
		AddPiece(new Grass(*this, x, y));
	}
}

World::~World()
{
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
	return mNumberOfTigers == 0;
}

bool World::IsGameOver() const
{
	return mHunter == nullptr;
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
	delete mHunter;
	mHunter = nullptr;
}

void World::Update(int command, int direction)
{
	// Hunter’s action → Tiger’s action → Rabbit’s action → Grass generation
	updateHunter(command, direction);
	updateTigers();
	updateRabbits();
	
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

			if (pCritter->GetLife() <= 0)
			{
				// 음식 위에 있었을 경우 토끼 자리에 음식을 배치
				Piece* obscured = pCritter->GetObscured();
				if (obscured != nullptr)
				{
					mGrid[i][j] = obscured;
					if (mHunter->GetLife() <= 0)
					{
						KillHunter();
					}
					else
					{
						delete pCritter;
					}
					continue;
				}
				
				RemovePiece(j, i);
				if (mHunter->GetLife() <= 0)
				{
					KillHunter();
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
	for (int i = 0; i < 50; ++i)
	{
		cout << "=";
	}
	cout << endl << endl;

	int hunterLife = 0;
	if (mHunter != nullptr)
	{
		hunterLife = mHunter->GetLife();
	}
	
	cout << "Time Step: " << mTimeStep << "\t\t" << "Life: " << hunterLife << endl;

	showGrid();
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
	// 10분의 1의 확률
	if (rand() % 10 > 0)
	{
		return;
	}

	int x, y;
	
	// 빈 칸을 획득하는데 실패할 경우 생략
	if (!getEmptyCell(x, y))
	{
		return;
	}

	// 획득한 빈 칸에 Grass 생성
	mGrid[y][x] = new Grass(*this, x, y);
}

void World::updateHunter(int command, int direction)
{	
	// Move
	if (command == 0)
	{
		mHunter->Move(direction);
	}
	// Shoot
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
