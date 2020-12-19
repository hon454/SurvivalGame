#pragma once

#include "Cell.h"
#include "Piece.h"

class World
{
public:
public:
	World(int H, int W, int G, int R, int T);
	~World();
	bool IsValid(int x, int y) const;
	bool IsEmpty(int x, int y) const;
	bool CanBreed(int x, int y) const;
	bool IsNoEmptyCell() const;
	bool HasRabbit(int x, int y) const;
	bool HasTiger(int x, int y) const;
	bool HasFood(int x, int y) const;
	bool HasGrass(int x, int y) const;

	bool AddPiece(Piece* piece);
	
	void Update();
	void Display() const;
private:
	Cell getEmptyCell() const;
	void generateGrass();
	void updateGrid();
private:
	int mHeight, mWidth;
	int mNumberOfGrasses, mNumberOfRabbits, mNumberOfTigers;
	int mTimeStep;
	Piece*** mGrid;
	Piece*** mNextGrid;
};



