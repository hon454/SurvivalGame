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
	// set grid
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

	// add hunter
	getEmptyCell(x, y);
	mHunter = new Hunter(*this, x, y);
	AddPiece(mHunter);

	// add rabbit
	for (int i = 0; i < R; ++i)
	{
		getEmptyCell(x, y);
		AddPiece(new Rabbit(*this, x, y));
	}

	// add tiger
	for (int i = 0; i < T; ++i)
	{
		getEmptyCell(x, y);
		AddPiece(new Tiger(*this, x, y));
	}

	// add grass
	for (int i = 0; i < G; ++i)
	{
		getEmptyCell(x, y);
		AddPiece(new Grass(*this, x, y));
	}
}

World::~World()
{
	// release memory
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
	for (int i = 0; i < mHeight; ++i)
	{
		for (int j = 0; j < mWidth; ++j)
		{
			// if tiger is exist, skip
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

	// restore obscured
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
	// update critter
	updateHunter(command, direction);
	updateTigers();
	updateRabbits();
	
	generateGrass();

	// decrease life, increase life step, kill starved critter
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

			// remove starved critter
			if (pCritter->GetLife() <= 0)
			{
				// if has obscured, restore
				Piece* obscured = pCritter->GetObscured();
				if (obscured != nullptr)
				{
					mGrid[i][j] = obscured;					
				}
				else
				{
					RemovePiece(j, i);
				}

				// if critter is hunter, switch mIsHunterStarved
				if (mHunter != nullptr && mHunter->GetLife() <= 0)
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
	// update when hunter is alive
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
	// if empty cell is not exist, skip
	if (!HasEmptyCell())
	{
		return false;
	}		

	// randomly select empty cell
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
			// generate grass by each empty cell, 10%
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

			// if is not tiger, skip
			if (pTiger == nullptr)
			{
				continue;
			}

			// if is already action, skip
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

			// if is not rabbit, skip
			if (pRabbit == nullptr)
			{
				continue;			
			}

			// if is already action, skip
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
