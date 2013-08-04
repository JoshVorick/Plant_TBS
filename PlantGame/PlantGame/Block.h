#pragma once

#include <allegro5\allegro_font.h>
#include <vector>
#include <string>
#include <map>
#include "Sprite.h"

#define NUM_SOIL_TYPES 4
#define MAX_PLAYERS 2
#define MAX_MINERALS 75
#define MAX_WATER 75
#define MAX_X 10
#define MAX_Y 10
#define MAX_Z 8

enum blockIDS {SOIL1, SOIL2, SOIL3, SOIL4}; //AIR is NULL pointer, see GameMap

class Block : public Sprite
{
private:
	//Images are held in GameMap
	bool isHoveredOn;
	bool isHighlighted;
	int blockID;				//Pretty obvious -- an ID describing what type of block it is
	int mineralsAvailable;		//How many minerals are currently available in the soil block
	int mineralsReplenishedPerTurn;	//How many minerals are given to the block after each turn cycle
	int waterAvailable;		//How much water is currently available in the soil block
	int waterReplenishedPerTurn;	//How much water is given to the block after each turn cycle

	std::map<int, int> numUnitRoots;	//How many roots each unit has in this block
	std::map<int, double> percentToGive; 

	void recalculatePercentages();	//Recalculaates percent of minerals allocated to each unit whenever roots get updated

public:
	Block(int id, int w, int h);	//Initializes values minearls/water based on ID passed
	~Block();	//Does whatever garbage collection is needed
	void toggleHighlight() {isHighlighted = !isHighlighted;}
	bool isHoveringOnBlock(int mouseX, int mouseY);
	int getBitmap() {return blockID;}	//Returns the bitmap so the block can be drawn
	void addRoots(int unitID);	//Used when block get new roots
	void growRoots(int unitID, int amtToGrow);
	double giveUnitMinerals(int unitID);	//See function definition for explanation
	double giveUnitWater(int unitID);
	//^Those could be ints maybe? Something to decide in the future once game is taking shape 
	void replenishResources();	//To be called after all players have taken their turns
	void drawInfoBox(ALLEGRO_FONT* font);
	void draw(ALLEGRO_BITMAP* image);
};