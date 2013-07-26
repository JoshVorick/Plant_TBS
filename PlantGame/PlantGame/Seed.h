/*
Seeds will be held in GameMap. This will make it easy to turn them into units because units are 
also in GameMap. Blocks may have a doesGerminate(int germChance/resilience) function
*/
#pragma once

#include <vector>
#include "Sprite.h"
#include "Block.h"

class Seed : public Sprite
{
private:
	int numOwnedBy[MAX_PLAYERS];

public:
	Seed() {}
	~Seed() {}
	bool hasSeeds(int playerNum) {} //Returns number of seeds 
};