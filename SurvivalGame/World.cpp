#include <iostream>

#include "World.h"
#include "Rabbit.h"
#include "Tiger.h"
#include "Hunter.h"
#include "Grass.h"
#include "Food.h"

using namespace std;

World::World(int H, int W, int G, int R, int T)
	:mHeight(H), mWidth(W), mNumberOfGrasses(G), mNumberOfRabbits(R), mNumberOfTigers(T), mTimeStep(0)
{
	mGrid = new Piece**[mHeight];
	mNextGrid = new Piece**[mHeight];
	
	for (int i = 0; i < mHeight; ++i)
	{
		mGrid[i] = new Piece*[mWidth];
		mNextGrid[i] = new Piece*[mWidth];
		for (int j = 0; j < mWidth; ++j)
		{
			mGrid[i][j] = nullptr;
			mNextGrid[i][j] = nullptr;
		}
	}

	Cell emptyCell = getEmptyCell();
	mHunter = new Hunter(*this, emptyCell.GetX(), emptyCell.GetY());
	AddPiece(mHunter);

	for (int i = 0; i < G; ++i)
	{
		emptyCell = getEmptyCell();
		AddPiece(new Grass(*this, emptyCell.GetX(), emptyCell.GetY()));
	}

	for (int i = 0; i < R; ++i)
	{
		emptyCell = getEmptyCell();
		AddPiece(new Rabbit(*this, emptyCell.GetX(), emptyCell.GetY()));
	}

	for (int i = 0; i < T; ++i)
	{
		emptyCell = getEmptyCell();
		AddPiece(new Tiger(*this, emptyCell.GetX(), emptyCell.GetY()));
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
		delete[] mNextGrid[i];
    }
    delete[] mGrid;
	delete[] mNextGrid;
}

bool World::IsValid(int x, int y) const
{
	return x >= 0 && x < mWidth&& y >= 0 && y < mHeight;
}

bool World::IsEmpty(int x, int y) const
{
	return mGrid[y][x] == nullptr && mNextGrid[y][x] == nullptr;
}

bool World::CanBreed(int x, int y) const
{
	return IsEmpty(x, y) || HasFood(x, y) || HasGrass(x, y);
}

bool World::HasEmptyCell() const
{
	bool hasEmptyCell = false;
	for (int i = 0; i < mHeight; ++i)
	{
		for (int j = 0; j < mWidth; ++j)
		{
			if (mGrid[i][j] == nullptr)
			{
				hasEmptyCell = true;
			}
		}
	}
	return hasEmptyCell;
}

bool World::HasRabbit(int x, int y) const
{
	if (mGrid[y][x] == nullptr)
	{
		return false;
	}

	return dynamic_cast<Critter*>(mGrid[y][x]) == nullptr;
}

bool World::HasTiger(int x, int y) const
{
	if (mGrid[y][x] == nullptr)
	{
		return false;
	}

	return dynamic_cast<Critter*>(mGrid[y][x]) == nullptr;
}

bool World::HasFood(int x, int y) const
{
	if (mGrid[y][x] == nullptr)
	{
		return false;
	}

	return dynamic_cast<Critter*>(mGrid[y][x]) == nullptr;
}

bool World::HasGrass(int x, int y) const
{
	if (mGrid[y][x] == nullptr)
	{
		return false;
	}

	return dynamic_cast<Grass*>(mGrid[y][x]) == nullptr;
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

void World::Update()
{
	// Hunter��s action �� Tiger��s action �� Rabbit��s action �� Grass generation
}

void World::Display() const
{
	for (int i = 0; i < 50; ++i)
	{
		cout << "=";
	}
	cout << endl << endl;

	cout << "Time Step: " << mTimeStep << "\t\t" << "Life: " << mHunter->GetLife() << endl;

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

Cell World::getEmptyCell() const
{
	if (!HasEmptyCell())
	{
		return Cell(-1, -1);
	}
	
	int x, y;
	
	while (true)
	{
		x = rand() % mWidth;
		y = rand() % mHeight;
		if (IsEmpty(x, y)) break;
	}

	return Cell(y, x);
}

void World::generateGrass()
{
	int probability = rand() % 10;

	if (probability == 0)
	{
		Cell cell = getEmptyCell();
		int x = cell.GetX();
		int y = cell.GetY();
		mNextGrid[y][x] = new Grass(*this, x, y);
	}
}

void World::updateGrid()
{
	for (int i = 0; i < mHeight; ++i)
	{
		for (int j = 0; j < mWidth; ++j)
		{
			mGrid[i][j] = mNextGrid[i][j];
			mNextGrid[i][j] = nullptr;
		}
	}
}