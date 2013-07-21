/*
Seeds will be held in GameMap. This will make it easy to turn them into units because units are 
also in GameMap. Blocks may have a doesGerminate(int germChance/resilience) function
*/
#pragma once

#include <vector>

class Seed
{
private:
	int playerNum;	//number of player who owns this seed
	int seedType;	//Which plant this seed will produce
	double germinationChance; //{0,1} chance it will germinate

public:
	Seed(int numPlayer, int plantType) {playerNum = numPlayer; seedType = plantType;}
	~Seed();
	int getPlayerNum() {return playerNum;}
};