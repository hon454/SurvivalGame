#pragma once

#include "Cell.h"
#include "Hunter.h"
#include "Piece.h"

class World
{
public:
public:
	World(int H, int W, int G, int R, int T);
	~World();
	bool IsValid(int x, int y) const;
	bool IsEmpty(int x, int y) const;
	bool IsGameWin() const;
	bool IsGameOver() const;
	bool CanBreed(int x, int y) const;
	bool HasEmptyCell() const;
	bool HasRabbit(int x, int y) const;
	bool HasTiger(int x, int y) const;
	bool HasFood(int x, int y) const;
	bool HasGrass(int x, int y) const;

	bool MovePiece(int x, int y, int newX, int newY);
	bool AddPiece(Piece* piece);
	
	void Update(int command, int direction);
	void Display() const;
private:
	Cell getEmptyCell() const;
	void generateGrass();
	void updateHunter(int command, int direction);
	void updateTigers();
	void updateRabbits();
	void updateGrid();
private:
	int mHeight, mWidth;
	int mNumberOfTigers;
	int mTimeStep;
	Piece*** mGrid;
	Piece*** mNextGrid;
	Hunter* mHunter;
};



