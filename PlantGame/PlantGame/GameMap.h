#pragma once

#include <vector>
#include "Block.h"
#include "Tree.h"
#include "Flower.h"
#include "Bush.h"
#include "Player.h"

enum CAMERA_ANGLES {NORTH_WEST, NORTH_EAST, SOUTH_WEST, SOUTH_EAST};
const int NUM_UNIT_TYPES = 2;	//num of units with images
const int NUM_IMAGES_PER_UNIT = 3;	//Number of images each unit has (sapling, budding, full-grown would be 3)

class GameMap
{
private:
	unsigned int numPlayers;		//Number of players that fit on this map
	std::vector<Player*> players;	//The players in this game
	Block *blockMap[11][11][9];	//Three dimensional game-world map. It is one bigger in each dir to make drawing easier
				//For now this is a 10x10x8 array to make it easier
	ALLEGRO_BITMAP *blockImages[NUM_SOIL_TYPES];
	ALLEGRO_BITMAP* unitImages[NUM_UNIT_TYPES][NUM_IMAGES_PER_UNIT];
	int unitWidths[NUM_UNIT_TYPES][NUM_IMAGES_PER_UNIT];
	int unitHeights[NUM_UNIT_TYPES][NUM_IMAGES_PER_UNIT];
	Unit* unitsOnMap[10][10]; //pointers to the units on the map so that game map has easier time knowing where/if units are
							  //updates when units are added
							  //possibly a better way to do this?
	int blockWidth;		//the width of the image
	int blockHeight;	//the height of the image
	int blockPerceivedHeight;	//the height that the block looks like it has in the image (if it were 3D)
	int x;	//Dimension along the east-west axis
	int y;	//Dimension along the north-south axis
	int z;	//Dimension along the up-down axis
	int angleID;
        
public:
	GameMap(int x, int y, int z);	//Creates an x by y by z map
	~GameMap();
	bool addPlayer(Player* newPlayer);
	void addUnit(int player, int x, int y);	//adds a unit at x,y owned by player
	std::vector<Player*> getPlayers() {return players;}
	int getNumPlayers() {return players.size();}

	void nextTurn(int nextPlayer); //Updates units and blocks for next player's turn
	void draw(int x, int y,int z);	//Draws based on camera position
};
