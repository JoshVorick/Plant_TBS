#pragma once

#include <vector>
#include "Block.h"

class Seed
{
private:
	int playerNum;	//number of player who owns this seed
	int seedType;	//Which plant this seed will produce
	double germinationChance; //{0,1} chance it will germinate
	Block *curLocation;		//The block this seed is on top of

public:
	Seed();
	~Seed();
	bool doesGerminate();	//returns if the seed will germinate. Takes into account block beneath's minerals
	Block* getLocation();
	void moveSeedTo(Block *newLoc);
};