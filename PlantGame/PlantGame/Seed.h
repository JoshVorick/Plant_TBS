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
	int numSeeds;

public:
	Seed() {numSeeds = 0;}
	~Seed() {}
	bool hasSeeds() {if(numSeeds > 0) return true; return false;} //Returns number of seeds 
	int getNumSeeds() {return numSeeds;}
	void addSeed() {numSeeds++;}
	void removeASeed() {numSeeds--;}
};