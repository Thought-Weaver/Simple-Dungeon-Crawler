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

/* The tiles for each place on the map */
struct map_tile
{
	char tilecharacter; /* The printed char for the tile */
	bool walkable;
	bool hiddentrap;
};

/* The structure for items being kept in inventory */
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

/* Set the size of the map */
int const mapwidth = 20;
int const mapheight = 20;

/* Create an empty map */
map_tile map[mapwidth][mapheight];

/* Set the inventory limit */
int const itemlimit = 10;

/* Create an empty inventory */
int inventory[itemlimit];

/* Create an empty array of items in the inventory */
inventoryitem item[itemlimit];

int currentlevel;

/* Creates the map at random and updates significant variables */
void generate_map()
{
	/* Update the current level */
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
	
	/* Create loot on the map */
	int loop = 0;
	while(loop < 5)
    	{
		srand(time(NULL));
		int l = rand() % 20;
		map[l][l].tilecharacter = 'L';
		map[l][l].walkable = true;
		loop++;
    	}
	
	/* Generate the door's position */
	int d = rand() % mapwidth;
	int e = rand() % mapheight;

	/* Update the map with the player at initial pos */
	map[d][e].tilecharacter = 'O';
	map[d][e].walkable = true;
	map[d][e].hiddentrap = false;
	map[0][0].tilecharacter = '.';
	map[0][0].walkable = true;
	map[0][0].hiddentrap = false;
}

/* Print out the current map */
void print_map()
{
	for(int y = 0; y < mapheight; y++)
	{
		for(int x = 0; x < mapwidth; x++)
		{
			if(x == playerpos.x && y == playerpos.y)
			{
				cout << "@";
			}
			else
			{
				cout << map[x][y].tilecharacter;
			}
		}
		if(y == 0)
		{
			cout << "   Dungeon Level: " << currentlevel;
		}
		cout << endl;
	}
}

/* Load saved data */
void load()
{
	ifstream myfile;
	myfile.open ("currentlevel.save", ios::out);

	/* Check to make sure the file can be opened */
	if(myfile.fail())
	{
		cout << "Error opening data..." << endl;
		myfile.close();
	}

	/* Read map info from the file */
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

/* Save the player's data */
void save()
{
	ofstream myfile;
	myfile.open ("currentlevel.save", ios::out);

	/* Check to make sure the file can be opened */
	if(myfile.fail())
	{
		cout << "Error opening data..." << endl;
		myfile.close();
	}

	/* Write map info to the file */
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

/* Save the player's loot */
void save_loot()
{
	srand(time(NULL));
	int loot = rand() % 5;
	ofstream myfile;
	myfile.open ("loot.save", ios::app);

	/* Check that the file can be opened */
	if(myfile.fail())
	{
		cout << "Error opening data..." << endl;
		myfile.close();
	}

	/* Write loot to file */
	if (myfile.is_open())
	{
		myfile << loot << endl;
	}

	myfile.close();
 }

/* Load the player's loot */
void load_loot()
{
	ifstream myfile;
	myfile.open ("loot.save", ios::in);

	/* Check that the file can be opened */
	if(myfile.fail())
	{
		cout << "Error opening data..." << endl;
		myfile.close();
	}

	/* Load the player's loot into inventory */
	if (myfile.is_open())
	{
		for(int l = 0; l < itemlimit; l++)
		{
			myfile >> inventory[l];
			switch (inventory[l])
			{
				 case 0: 
					item[l].id = 0; 
					item[l].equipped = false; 
					item[l].name = "Empty"; 
					break;
			     	 case 1: 
					item[l].id = 1; 
					item[l].equipped = false; 
					item[l].name = "Boots"; 
					break;
				 case 2: 
					item[l].id = 2; 
					item[l].equipped = false; 
					item[l].name = "Shield"; 
					break;
				 case 3: 
					item[l].id = 3; 
					item[l].equipped = false; 
					item[l].name = "Sword"; 
					break;
				 case 4: 
					item[l].id = 4; 
					item[l].equipped = false; 
					item[l].name = "Torso Armor"; 
					break;
			     case 5: 
					item[l].id = 5; 
					item[l].equipped = false; 
					item[l].name = "Helm"; 
					break;
			     default: 
					break;
			}
		}
	}

	 myfile.close();
 }

/* Save additional misc data */
void save_other_data()
{
	ofstream myfile;
	myfile.open ("otherdata.save", ios::out);

	/* Check that the file can be opened */
	if(myfile.fail())
	{
		cout << "Error opening data..." << endl;
		myfile.close();
	}

	/* Write additional data to file */
	if (myfile.is_open())
	{
		myfile << currentlevel << endl;
	}

	 myfile.close();
 }

void load_other_data()
{
	ifstream myfile;
	myfile.open ("otherdata.save", ios::in);

	/* Check that the file can be opened */
	if(myfile.fail())
	{
		cout << "Error opening data..." << endl;
		myfile.close();
	}

	/* Load additional data from file */
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
	/* Play the background music */
	PlaySound(TEXT("mainmusic.wav"), NULL, SND_FILENAME | SND_ASYNC);
	
	/* Set player's initial position to 0, 0 */
	playerpos.x = 0;
	playerpos.y = 0;
	
	/* Load saved data */
	load();
	load_loot();
	load_other_data();

	char option = 'w';

	/* Game loop */
	while(true)
	{
	
	/* Print map and instructions */
	print_map();
	cout << "WASD to move, I to view your inventory, and E to save." << endl;
	cout << ">";
	cin >> option;

	/* Handle movement input */
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

	/* If moving at edges of the map */
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

	/* Equip an item if the selected item is available to equip */
	if(option <= itemlimit)
	{
		if(item[option].equipped != true && item[option].id != 0) 
			item[option].equipped = true;
	}

	/* Save data */
	if(option == 'E' || option == 'e')
	{
		save();
		save_other_data();
	}

	/* If the player reaches the door, generate the next level */
	if(map[playerpos.x][playerpos.y].tilecharacter == 'O')
	{
		save();
		save_other_data();
		generate_map();
		playerpos.x = 0;
		playerpos.y = 0;
	}

	/* If the player manually wants to generate a new level */
	if(option == 'G' || option == 'g')
	{
		generate_map();
		playerpos.x = 0;
		playerpos.y = 0;
	}

	/* If the player finds loot */
	if(map[playerpos.x][playerpos.y].tilecharacter == 'L')
	{
		map[playerpos.x][playerpos.y].tilecharacter = '.';
		cout << "Loot collected!" << endl << endl;
		save_loot();
	}

	/* If the player triggers a trap, send them back to where they started */
	if(map[playerpos.x][playerpos.y].tilecharacter == '.' && map[playerpos.x][playerpos.y].hiddentrap == true)
	{
		map[playerpos.x][playerpos.y].tilecharacter = 'X';
		cout << "Trap triggered! Head back to the entrance." << endl << endl;
		playerpos.x = 0;
		playerpos.y = 0;
	}

	/* If the player quits the game */
	if(option == 'Q') 
		break;

	/* If the player wants to view their inventory */
	if(option == 'I' || option == 'i')
	{
		load_loot();
		cout << "Inventory: " << endl;

		for(int l = 0; l < itemlimit; l++)
		{
			cout << item[l].name << endl;
			cout << item[l].equipped << endl;
		}

		cout << endl;
	}

	}

	_getch();
	return 0;
}
