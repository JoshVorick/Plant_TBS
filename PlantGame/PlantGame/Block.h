#pragma once

#include <allegro5\allegro_font.h>
#include <vector>
#include <string>
#include "Sprite.h"

#define NUM_SOIL_TYPES 4
#define MAX_PLAYERS 2
#define MAX_MINERALS 300
#define MAX_MINERALS 300

enum blockIDS {SOIL1, SOIL2, SOIL3, SOIL4}; //AIR is NULL pointer, see GameMap

class Block : public Sprite
{
private:
	//Images are held in GameMap
	bool isHoveredOn;
	int blockID;				//Pretty obvious -- an ID describing what type of block it is
	int mineralsAvailable;		//How many minerals are currently available in the soil block
	int mineralsGivenPerTurn;		//Amount given total to players, determined by Sigmoid(sum of numUnitRoots){0,1} * mineralsAvailable
	int mineralsReplenishedPerTurn;	//How many minerals are given to the block after each turn cycle
	int waterAvailable;		//How much water is currently available in the soil block
	int waterGivenPerTurn;	//like mineralsGivenPerTurn
	int waterReplenishedPerTurn;	//How much water is given to the block after each turn cycle

	std::vector<int> numUnitRoots;	//Array of intes giving the number of roots of each unit in the block
	//Each unit has an id, given to it at creation. Every time a unit is created, an int is added (starting at 0) for every block

public:
	Block(int id, int w, int h);	//Initializes values minearls/water based on ID passed
	~Block();	//Does whatever garbage collection is needed
	bool isHoveringOnBlock(int mouseX, int mouseY);
	int getBitmap() {return blockID;}	//Returns the bitmap so the block can be drawn
	void addRoots(int unitID);
	void growRoots(int unitID, int amtGrown);
	double giveUnitMinerals(int unitID);	//See function definition for explanation
	double giveUnitWater(int unitID);
	//^Those could be ints maybe? Something to decide in the future once game is taking shape 
	void replenishResources();	//To be called after all players have taken their turns
	void drawInfoBox(ALLEGRO_FONT* font);
	void draw(ALLEGRO_BITMAP* image);
};