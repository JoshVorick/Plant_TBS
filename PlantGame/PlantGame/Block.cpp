#pragma once
#include "Block.h"

Block::Block(int id, int w, int h)	//Initializes values minearls/water based on ID passed
{
	Sprite();
	blockID = id;
	width = w;
	height = h;
	switch(id){
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

bool Block::isHoveringOnBlock(int mouseX, int mouseY){
	mouseX-=width/2;
	mouseY-=height/2;
	if(abs(mouseX-x) + abs(mouseY-y) < width/2){
		isHoveredOn=true;
		return true;
	}
	isHoveredOn=false;
	return false;
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

void Block::drawInfoBox(ALLEGRO_FONT* font){
	al_draw_textf(font, al_map_rgb(25, 200, 100), x+50, y+25, 0, "Minerals: %i Minerals Per Turn: %i", mineralsAvailable, mineralsReplenishedPerTurn);
}

void Block::draw(ALLEGRO_BITMAP* image){
	if(!isHoveredOn)
		al_draw_bitmap(image, x, y, 0);
	else
		al_draw_tinted_bitmap(image, al_map_rgb(100,100,100), x, y, 0);
}