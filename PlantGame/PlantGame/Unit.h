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
	int xLoc, yLoc; //the integer location of the units on the game map (0-10)
	int classID;	//Determines whether the unit is a flower, bush, or tree
	int unitID;	//ID for this unit
	int hitPoints;	//Number of hitpoints the unit has
	int maxHitPoints;	//Max hp of a unit
	int level;		//Level of plant. Increases with mineral surplus
	int size;
	int playerNum;	//Pointer to the player who owns the plant
	int resilience;	//Maybe a measure of how hardy the plant is? How much it takes for it to start dying?
	int mineralsToProduceASeed;	//minerals needed to make a seed
	int waterToProduceASeed;	//water needed to make a seed
	int mineralsToLevelUp;	//same^
	int waterToLevelUp;		//same^
	int mineralsStored; //stores excess minerals
	int waterStored;	//same^
	Block* blockMap[MAX_X+1][MAX_Y+1][MAX_Z+1]; //pointer to the blocks on the map
	std::vector<Block*> hasRootsIn; //Points to which blocks the unit has roots in

public:
	Unit();
	~Unit();	//Garbage collection
	void toggleSelected() {for(unsigned int i=0; i<hasRootsIn.size(); i++) hasRootsIn.at(i)->toggleHighlight();}
	void setBlockMap(Block* blocks[MAX_X+1][MAX_Y+1][MAX_Z+1], int unitX, int unitY); //sets pointer where they should point
	void setID(int ID) {unitID = ID;}
	int getID() {return unitID;}
	int getClass() {return classID;}
	int getSize() {return size;}
	int getCurrentHitPoints() {return hitPoints;}	//Returns the plant's current hit points
	int getMaxHitPoints() {return maxHitPoints;}	//Returns the max hp of the plant
	void setOwner(int playerNum) {this->playerNum = playerNum;}	//Sets the player who owns the plant
	int getOwner() {return playerNum;}	//Returns the owner of the plant
	int getResilience() {return resilience;}	//Returns the defense stat of the plant
	bool makeSeed();
	bool levelUp();
	bool makeASeed();
	void virtual addMinerals();	//Gets minerals from its soil blocks. Also calculates if it needs to make seeds and calculates level and stuff
													//Returns number of seeds dropped/created
													//Returns -1 if it doesnt grow nor make seeds
	//void virtual draw(ALLEGRO_BITMAP* image){ al_draw_tinted_bitmap(image, al_map_rgba(255, 255, 255, 225), x, y, 0);}
	void virtual drawInfoBox(ALLEGRO_FONT* font) {al_draw_textf(font, al_map_rgb(30,130,230), 25, 200, 0, "Level: %i Minerals: %i Water: %i", 
		level, mineralsStored, waterStored);
		al_draw_textf(font, al_map_rgb(30,130,230), 25, 235, 0, "Minerals needed to level up: %i", mineralsToLevelUp);
		al_draw_textf(font, al_map_rgb(30,130,230), 25, 270, 0, "Minerals needed to make a seed: %i", mineralsToProduceASeed);
		al_draw_textf(font, al_map_rgb(30,130,230), 25, 305, 0, "Type: %i", classID);}
};