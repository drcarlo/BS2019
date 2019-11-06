#include <iostream>
#include <fstream>
#include <time.h>

using namespace std;

// two different display possibilities
char dispArray[] = { '~', 'S', 'O', 'X' };
char dispArray2[] = { '~', '~', 'O', 'X' };

// class board
class board
{
	enum cell { sea = 0, ship = 1, hit = 2, miss = 3 };
	unsigned int size;
	int** map;
	unsigned int shipNum; 
	unsigned int hitShips; 
	unsigned int score; 
public:
	board(unsigned int _size, unsigned int _shipNum)
	{
		// assign the vars
		size = _size; 
		hitShips = 0;
		score = 0; 
		// test for out of bounds
		if (_shipNum > size* size)
			_shipNum = size; 
		shipNum = _shipNum; 
		// allocate memory for grid
		map = new int* [size];
		for (unsigned int i = 0; i < size; i++)
			map[i] = new int[size];

		for (unsigned int i = 0; i < size; i++)
			for (unsigned int j = 0; j < size; j++)
				map[i][j] = sea; 

		// place ships
		initialiseShips(); 

	}

	~board() {
		// destroy everything
		for (unsigned int i = 0; i < size; i++)
			delete[] map[i];
		delete[] map; 
	}

	void display() {
		// clear the screen
		system("cls");
		for (unsigned int i = 0; i < size; i++) {
			for (unsigned int j = 0; j < size; j++)
				cout << dispArray[map[i][j]];
				//cout << dispArray2[map[i][j]];
			cout << endl;
		}
	}

	void save() {
		cout << "Saving map..." << endl;
		system("pause");
		ofstream fout("map.txt");
		for (unsigned int i = 0; i < size; i++) {
			for (unsigned int j = 0; j < size; j++) {
				fout << map[i][j];
			}
			fout << endl;
		}
		fout.close();
	}

	void load() {
		cout << "Loading new map..." << endl;
		system("pause");
		ifstream fin("map.txt");
		for (unsigned int i = 0; i < size; i++) {
			for (unsigned int j = 0; j < size; j++) {
				map[i][j] = fin.get() - 48; // Convert ASCII code to 0
			}
			fin.ignore(); // Make sure to ignore the newline (std::endl or '\n')
		}
		fin.close();
	}

	bool tryHit(unsigned int x, unsigned int y)
	{
		// evaluate this before checking bounds as this config is invariably > bounds
		if (x == 9 && y == 9) { // Nine Nine
			save();
			return false; // return before score increment as we're not attempting a valid hit
		}

		if (x == 8 && y == 8) {
			load();
			return false;
		}

		// check for bounds
		if ((x < 0 || x >= size) || (y < 0 || y >= size))
			return false;

		score++; 
		// check if hit test
		if (map[x][y] == ship)
		{
			map[x][y] = hit;
			hitShips++; 
		}
		else if (map[x][y] == sea)
			map[x][y] = miss; 

		return true; 
	}

	bool terminate()
	{
		return hitShips == shipNum; 
	}

	unsigned int getScore()
	{
		return score; 
	}

private:
	// place the ship - return success or not
	bool placeShip(unsigned int x, unsigned int y)
	{
		if (map[x][y] != ship) {
			map[x][y] = ship;
			return true;
		}
		else return false;
	}

	//initialise ships
	void initialiseShips()
	{
		// places all ships
		for (unsigned int i = 0; i < shipNum; i++) {
			unsigned int x, y;
			// test whether ship is placed in the right place
			do {
				x = rand() % size;
				y = rand() % size;
			} while (!placeShip(x, y));
		}
	}
};

int main()
{
	srand((unsigned int )time(NULL));

	unsigned int size, shipNum; 
	// no test for sizes here - thrust to user to get it right
	cout << "Input size and ship num: "; 
	cin >> size >> shipNum;

	board b(size, shipNum); 
	b.display();

	// game loop 
	do {
		unsigned int x, y;
		do {
			b.display(); // to ensure we keep updating after saves and loads
			cout << "Input Coordinates: ";
			cin >> x >> y;
		} while (!b.tryHit(x, y));
		b.display();
	} while (!b.terminate()); 

	cout << endl << "Final Score is: " << b.getScore() << endl;;
	return 0; 
}