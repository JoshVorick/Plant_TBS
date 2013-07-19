#pragma once
#include "Block.h"

Block::Block(int id)	//Initializes values minearls/water based on ID passed
{
	blockID = id;
	switch(id)
	{
	case 1:
		mineralsAvailable = 250;
		mineralsGivenPerTurn = 0;	//No roots yet, so none given
		mineralsReplenishedPerTurn = 100;
		waterAvailable = 100;
		waterGivenPerTurn = 0;
		waterReplenishedPerTurn = 30;
		break;
	case 2:
		mineralsAvailable = 50;
		mineralsGivenPerTurn = 0;	//No roots yet, so none given
		mineralsReplenishedPerTurn = 150;
		waterAvailable = 150;
		waterGivenPerTurn = 0;
		waterReplenishedPerTurn = 50;
		break;
	case 3:
		mineralsAvailable = 150;
		mineralsGivenPerTurn = 0;	//No roots yet, so none given
		mineralsReplenishedPerTurn = 125;
		waterAvailable = 75;
		waterGivenPerTurn = 0;
		waterReplenishedPerTurn = 30;
		break;
	case 4:
		mineralsAvailable = 350;
		mineralsGivenPerTurn = 0;	//No roots yet, so none given
		mineralsReplenishedPerTurn = 80;
		waterAvailable = 10;
		waterGivenPerTurn = 0;
		waterReplenishedPerTurn = 20;
		break;
	}
}

Block::~Block()	//Does whatever garbage collection is needed
{
}

int Block::getBitmap()	//Returns the bitmap so the block can be drawn
{
	return blockID;
}

double Block::giveUnitMinerals(int unitID)	//See function definition for explanation
{
	/*	
		Each player will be assigned a number
		Say there are four players in a game, they will be 0 through 3, taking turns in that order
		This gives player number playerNum the percentage of the resources they are owed
		It also deducts the amount from what is available to the block
		
		The materials will be given to the player like this:
		Player.addMinerals(someBlock.givePlayerMinerals(Player.getPlayerNumber()));
	*/
	double percentage = numUnitRoots[unitID];
	double amountToGive = percentage * mineralsGivenPerTurn;
	mineralsAvailable -= amountToGive;
	return amountToGive;
}

double Block::giveUnitWater(int unitID)
{
	double percentage = numUnitRoots[unitID];
	double amountToGive = percentage * waterGivenPerTurn;
	waterAvailable -= amountToGive;
	return amountToGive;
}

void Block::replenishResources()
{
	mineralsAvailable += mineralsReplenishedPerTurn;
	waterAvailable += waterReplenishedPerTurn;
}