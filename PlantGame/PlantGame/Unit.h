#pragma once

#include <vector>
#include <allegro5\allegro_font.h>
#include "Block.h"
#include "Player.h"
#include "Sprite.h"

class Player;
enum UNIT_CLASSES {TREE, FLOWER, BUSH};

class Unit : public Sprite
{
protected:
	int classID;	//Determines whether the unit is a flower, bush, or tree
	int hitPoints;	//Number of hitpoints the unit has
	int maxHitPoints;	//Max hp of a unit
	int level;		//Level of plant. Increases with mineral surplus
	int size;
	int playerNum;	//Pointer to the player who owns the plant
	int resilience;	//Maybe a measure of how hardy the plant is? How much it takes for it to start dying?
	int seeds;	//How many seeds are produced by the plant per cycle of seed production
	int mineralsToProduceASeed;	//minerals needed to make a seed
	int waterToProduceASeed;	//water needed to make a seed
	int mineralsToLevelUp;	//same^
	int waterToLevelUp;		//same^
	int mineralsStored; //stores excess minerals
	int waterStored;	//same^

public:
	Unit();
	~Unit();	//Garbage collection

	int getClass() {return classID;}
	int getSize() {return size;}
	int getCurrentHitPoints() {return hitPoints;}	//Returns the plant's current hit points
	int getMaxHitPoints() {return maxHitPoints;}	//Returns the max hp of the plant
	void setOwner(int playerNum) {this->playerNum = playerNum;}	//Sets the player who owns the plant
	int getOwner() {return playerNum;}	//Returns the owner of the plant
	int getResilience() {return resilience;}	//Returns the defense stat of the plant
	int virtual addMinerals(int mineralsToAdd);	//Gets minerals from its soil blocks. Also calculates if it needs to make seeds and calculates level and stuff
													//Returns number of seeds dropped/created
													//Returns -1 if it doesnt grow nor make seeds
	void virtual drawInfoBox(ALLEGRO_FONT* font) {al_draw_textf(font, al_map_rgb(30,130,230), x+width, y, 0, "Level: %i Minerals: %i Water: %i", 
		level, mineralsStored, waterStored);}
};