#include <iostream>
#include <cassert>

#include "World.h"

using namespace std;

int main(void)
{
	srand(time(NULL));
	
	int H, W, G, R, T;
	
	cout << "Enter grid height: ";
	cin >> H;
	assert(1 <= H && H <= 1000);

	cout << "Enter grid width: ";
	cin >> W;
	assert(1 <= W && W <= 1000);

	cout << endl;

	cout << "Enter initial number of grasses: ";
	cin >> G;
	assert(G >= 0);
	
	cout << "Enter initial number of rabbits: ";
	cin >> R;
	assert(R >= 1);
	
	cout << "Enter initial number of tigers: ";
	cin >> T;
	assert(T >= 1);

	assert(G + R + T + 1 <= H * W);

	World world(H, W, G, R, T);

	int command;
	int direction;
	
	while(true)
	{
		world.Display();

		if (world.IsGameOver())
		{
			cout << "\nGame Over" << endl;
			break;
		}
		else if (world.IsGameWin())
		{
			cout << "\nGame Win" << endl;
			break;
		}

		cout << "Select next command [0: move / 1: shoot]: ";
		cin >> command;
		assert(command == 0 || command == 1);

		cout << "Enter the direction [0:up / 1: down / 2: right / 3: left]: ";
		cin >> direction;
		assert(0 <= direction || direction <= 3);

		world.Update(command, direction);
	}
}