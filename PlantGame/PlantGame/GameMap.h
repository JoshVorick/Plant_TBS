#pragma once

#include <vector>
#include "Block.h"
#include "Tree.h"
#include "Flower.h"
#include "Bush.h"
#include "Player.h"
#include "Seed.h"

enum CAMERA_ANGLES {NORTH_WEST, NORTH_EAST, SOUTH_WEST, SOUTH_EAST};
#define NUM_UNIT_TYPES		2	//num of units with images
#define NUM_IMAGES_PER_UNIT 3	//Number of images each unit has (sapling, budding, full-grown would be 3)
#define UNEVEN_LAYERS		3	//layers that can have NULL spaces
#define PLANT_UPGRADE_X		25	//x of where plant 'menu' to grow/make a seed is
#define PLANT_UPGRADE_Y		140	//y distance from bottom edge of menu
#define WIDTH				1920
#define HEIGHT				1080

class GameMap
{
private:
	int numPlayers;		//Number of players that fit on this map
	int curPlayer;			//number of the player taking their turn
	int nextUnitID; //ID to be given to next unit
	std::vector<Player*> players;	//The players in this game
	std::vector<Unit*> units; //array of initialized units
	Block* blockMap[MAX_X+1][MAX_Y+1][MAX_Z+1];	//Three dimensional game-world map. It is one bigger in each dir to make drawing easier
				//For now this is a 10x10x8 array to make it easier
	Unit* unitsOnMap[MAX_X][MAX_Y]; //pointers to the units on the map so that game map has easier time knowing where/if units are
							  //updates when units are added
							  //possibly a better way to do this?
	Seed* seedsOnMap[MAX_X][MAX_Y][MAX_PLAYERS];

	ALLEGRO_BITMAP* blockImages[NUM_SOIL_TYPES];
	ALLEGRO_BITMAP* unitImages[NUM_UNIT_TYPES][NUM_IMAGES_PER_UNIT];
	ALLEGRO_BITMAP* seedImages[NUM_UNIT_TYPES];	//one seed image per type of unit
	int unitWidths[NUM_UNIT_TYPES][NUM_IMAGES_PER_UNIT];
	int unitHeights[NUM_UNIT_TYPES][NUM_IMAGES_PER_UNIT];
	int blockWidth;		//the width of the image
	int blockHeight;	//the height of the image
	int seedWidths[NUM_UNIT_TYPES];
	int seedHeights[NUM_UNIT_TYPES];
	int blockPerceivedHeight;	//the height that the block looks like it has in the image (if it were 3D)
	
	int blockMouseIsOn[3]; //3 members hold x,y,z of block mouse is on top of
	int selectedUnitCoor[2];
	bool unitOnBlock; //Is there a unit on that^ block?
	Unit* selectedUnit; //points to the selected unit. NULL if none are selected
	Block* blockUnderSelectedUnit; //The block under that^ unit
	bool autopilot; //Automatically levels up and grows seeds for the plant

	int x;	//Dimension along the east-west axis
	int y;	//Dimension along the north-south axis
	int z;	//Dimension along the up-down axis
	int angleID;
	
	void addSeed(int x, int y);

public:
	GameMap(int x, int y, int z);	//Creates an x by y by z map
	~GameMap();
	bool addPlayer(Player* newPlayer);
	void addUnit(int player, int x, int y);	//adds a unit at x,y owned by player
	std::vector<Player*> getPlayers() {return players;}
	int getCurPlayerNum() {return curPlayer;}
	int getNumPlayers() {return players.size();}

	void mouseClick(int mouseX, int mouseY);
	void changeCamera(int dx, int dy, int dz, double dZoom);
	void nextTurn(); //Updates units and blocks for next player's turn
	void draw(int x, int y,int z, double zoom, ALLEGRO_FONT* font, int mouseX, int mouseY);	//Draws based on camera position
};