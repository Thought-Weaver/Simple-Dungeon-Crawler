#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

using namespace std;

struct map_tile
{
	char tilecharacter;
	bool walkable;
	bool hiddentrap;
};

struct inventoryitem
{
	bool equipped;
	int id;
	string name;
};

struct Point
{
	int x, y;
}
playerpos;

int const mapwidth = 20;
int const mapheight = 20;

map_tile map[mapwidth][mapheight];

int const itemlimit = 10;

int inventory[itemlimit];

inventoryitem item[itemlimit];

int currentlevel;

void GenerateMap()
{
	currentlevel = currentlevel + 1;
	srand(time(NULL));
	for(int y = 0; y < mapheight; y++)
	{
		for(int x = 0; x < mapwidth; x++)
		{
			int t = rand() % 3;
			if(t == 0)
			{
				map[x][y].tilecharacter = '#';
				map[x][y].walkable = false;
				map[x][y].hiddentrap = false;
			}
			else
			{
				map[x][y].tilecharacter = '.';
			    map[x][y].walkable = true;
				map[x][y].hiddentrap = false;
			}

			int r = rand() % 20;
			if(r == 1)
			{
				map[x][y].tilecharacter = '.';
				map[x][y].walkable = true;
				map[x][y].hiddentrap = true;
			}
		}
	}
	
	int loop = 0;
    while(loop < 5)
    {
		srand(time(NULL));
		int l = rand() % 20;
		map[l][l].tilecharacter = 'L';
		map[l][l].walkable = true;
		loop++;
    }

	int d = rand() % mapwidth;
	int e = rand() % mapheight;

	map[d][e].tilecharacter = 'O';
	map[d][e].walkable = true;
	map[d][e].hiddentrap = false;
	map[0][0].tilecharacter = '.';
	map[0][0].walkable = true;
	map[0][0].hiddentrap = false;
}

void PrintMap()
{
	for(int y = 0; y < mapheight; y++)
	{
		for(int x = 0; x < mapwidth; x++)
		{
				if(x == playerpos.x && y == playerpos.y)
					cout << "@";
				else
					cout << map[x][y].tilecharacter;
		}
		if(y == 0)
		{
			cout << "   Dungeon Level: " << currentlevel;
		}
		cout << endl;
	}
}

void load()
{
	ifstream myfile;
	myfile.open ("currentlevel.save", ios::out);

	if(myfile.fail())
	{
		cout << "Error opening data..." << endl;
		myfile.close();
	}

	if (myfile.is_open())
  {
     for(int y = 0; y < mapheight; y++)
	{
		for(int x = 0; x < mapwidth; x++)
		{
           myfile >> map[x][y].tilecharacter;
           myfile >> map[x][y].walkable;
		}
     }
		 myfile.close();
  }
}

void save()
{
	ofstream myfile;
	myfile.open ("currentlevel.save", ios::out);

	if(myfile.fail())
	{
		cout << "Error opening data..." << endl;
		myfile.close();
	}

	if (myfile.is_open())
  {
     for(int y = 0; y < mapheight; y++)
	{
		for(int x = 0; x < mapwidth; x++)
		{
           myfile << map[x][y].tilecharacter;
           myfile <<  map[x][y].walkable;
		}
     }
		 myfile.close();
  }
}

void saveloot()
{
	srand(time(NULL));
	int loot = rand() % 5;
	ofstream myfile;
	myfile.open ("loot.save", ios::app);

	if(myfile.fail())
	{
		cout << "Error opening data..." << endl;
		myfile.close();
	}

	if (myfile.is_open())
		{
			myfile << loot << endl;
		}

	 myfile.close();
 }

void loadloot()
{
	ifstream myfile;
	myfile.open ("loot.save", ios::in);

	if(myfile.fail())
	{
		cout << "Error opening data..." << endl;
		myfile.close();
	}

	if (myfile.is_open())
		{
			for(int l = 0; l < itemlimit; l++)
			{
				myfile >> inventory[l];
				switch (inventory[l])
				{
					 case 0: item[l].id = 0; item[l].equipped = false; item[l].name = "Empty"; break;
				     case 1: item[l].id = 1; item[l].equipped = false; item[l].name = "Boots"; break;
					 case 2: item[l].id = 2; item[l].equipped = false; item[l].name = "Shield"; break;
					 case 3: item[l].id = 3; item[l].equipped = false; item[l].name = "Sword"; break;
					 case 4: item[l].id = 4; item[l].equipped = false; item[l].name = "Torso Armor"; break;
				     case 5: item[l].id = 5; item[l].equipped = false; item[l].name = "Helm"; break;
				     default: break;
				}
			}
		}

	 myfile.close();
 }

void saveotherdata()
{
	ofstream myfile;
	myfile.open ("otherdata.save", ios::out);

	if(myfile.fail())
	{
		cout << "Error opening data..." << endl;
		myfile.close();
	}

	if (myfile.is_open())
		{
			myfile << currentlevel << endl;
		}

	 myfile.close();
 }

void loadotherdata()
{
	ifstream myfile;
	myfile.open ("otherdata.save", ios::in);

	if(myfile.fail())
	{
		cout << "Error opening data..." << endl;
		myfile.close();
	}

	if (myfile.is_open())
		{
			for(int loadlines = 0; loadlines < 1; loadlines++)
			{
				if(loadlines == 0)
				{
				myfile >> currentlevel;
				}
			}
		}

	 myfile.close();
 }

int main()
{
	PlaySound(TEXT("mainmusic.wav"), NULL, SND_FILENAME | SND_ASYNC);
	playerpos.x = 0;
	playerpos.y = 0;
	load();
	loadloot();
	loadotherdata();

	char option = 'w';

	while(true)
	{
	PrintMap();
	cout << "WASD to move, I to view your inventory, and E to save." << endl;
	cout << ">";
	cin >> option;

	if((option == 'W' || option == 'w') && playerpos.y > 0 && map[playerpos.x][playerpos.y-1].walkable)
	{
		system("cls");
		playerpos.y--;
	}
	if((option == 'S' || option == 's') && playerpos.y < mapheight-1 && map[playerpos.x][playerpos.y+1].walkable)
	{
		system("cls");
		playerpos.y++;
	}
	if((option == 'D' || option == 'd') && playerpos.x < mapwidth-1 && map[playerpos.x+1][playerpos.y].walkable)
	{
		system("cls");
		playerpos.x++;
	}
	if((option == 'A' || option == 'a') && playerpos.x > 0 && map[playerpos.x-1][playerpos.y].walkable)
	{
		system("cls");
		playerpos.x--;
	}


	if((option == 'W' || option == 'w') && playerpos.y > 0 || map[playerpos.x][playerpos.y-1].walkable == false)
	{
		system("cls");
	}
	if((option == 'S' || option == 's') && playerpos.y < mapheight-1 || map[playerpos.x][playerpos.y+1].walkable == false)
	{
		system("cls");
	}
	if((option == 'D' || option == 'd') && playerpos.x < mapwidth-1 || map[playerpos.x+1][playerpos.y].walkable == false)
	{
		system("cls");
	}
	if((option == 'A' || option == 'a') && playerpos.x > 0 || map[playerpos.x-1][playerpos.y].walkable == false)
	{
		system("cls");
	}

	if(option <= itemlimit)
	{
		if(item[option].equipped != true && item[option].id != 0) item[option].equipped = true;
	}


	if(option == 'E' || option == 'e')
	{
		save();
		saveotherdata();
	}

	if(map[playerpos.x][playerpos.y].tilecharacter == 'O')
	{
		save();
		saveotherdata();
		GenerateMap();
		playerpos.x = 0;
		playerpos.y = 0;
	}

	if(option == 'G' || option == 'g')
	{
		GenerateMap();
		playerpos.x = 0;
		playerpos.y = 0;
	}

	if(map[playerpos.x][playerpos.y].tilecharacter == 'L')
	{
		map[playerpos.x][playerpos.y].tilecharacter = '.';
		cout << "Loot collected!" << endl << endl;
		saveloot();
	}

	if(map[playerpos.x][playerpos.y].tilecharacter == '.' && map[playerpos.x][playerpos.y].hiddentrap == true)
	{
		map[playerpos.x][playerpos.y].tilecharacter = 'X';
		cout << "Trap triggered! Head back to the entrance." << endl << endl;
		playerpos.x = 0;
		playerpos.y = 0;
	}

	if(option == 'Q') break;

	if(option == 'I' || option == 'i')
	{
	loadloot();
	cout << "Inventory: " << endl;

		for(int l = 0; l < itemlimit; l++)
		{
			cout << item[l].name << endl;
			cout << item[l].equipped << endl;
		}

	cout << endl;

	}

	if(option == 'N' || option == 'n')
	{
		save();
		saveotherdata();
	}


	}

	_getch();
	return 0;
}