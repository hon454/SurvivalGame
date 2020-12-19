#pragma once

class Cell
{
public:
	Cell(int x, int y) : mX(x), mY(y) {}
	int GetX() const { return mX; }
	int GetY() const { return mY; }
private:
	int mX, mY;
};