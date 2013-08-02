#pragma once
#include "Block.h"

Block::Block(int id, int w, int h){
	Sprite();
	blockID = id;
	width = w;
	height = h;
	switch(id){
		case 1:
			mineralsAvailable = 25;
			mineralsReplenishedPerTurn = 10;
			waterAvailable = 10;
			waterReplenishedPerTurn = 3;
			break;
		case 2:
			mineralsAvailable = 5;
			mineralsReplenishedPerTurn = 15;
			waterAvailable = 15;
			waterReplenishedPerTurn = 5;
			break;
		case 3:
			mineralsAvailable = 15;
			mineralsReplenishedPerTurn = 12;
			waterAvailable = 7;
			waterReplenishedPerTurn = 3;
			break;
		case 4:
			mineralsAvailable = 35;
			mineralsReplenishedPerTurn = 8;
			waterAvailable = 1;
			waterReplenishedPerTurn = 2;
			break;
	}
}

Block::~Block()
{
}

void Block::recalculatePercentages(){

}

bool Block::isHoveringOnBlock(int mouseX, int mouseY){
	mouseX-=width/2;
	mouseY-=height/2;
	if(abs(mouseX-x) + abs(mouseY-y) < width/2-16){
		return true;
	}
	isHoveredOn=false;
	return false;
}

void Block::addRoots(int unitID){
	numUnitRoots[unitID] = 10;
	percentToGive[unitID] = 0.9;
	recalculatePercentages();
}

void Block::growRoots(int unitID, int amtToGrow){
	numUnitRoots[unitID] += amtToGrow;
	recalculatePercentages();
}

double Block::giveUnitMinerals(int unitID)
{
	int amtToGive = percentToGive[unitID] * mineralsAvailable;
	mineralsAvailable -= amtToGive;
	return amtToGive;
}

double Block::giveUnitWater(int unitID)
{
	return -1;
}

void Block::replenishResources()
{
	mineralsAvailable += mineralsReplenishedPerTurn;
	if(mineralsAvailable > MAX_MINERALS) mineralsAvailable = MAX_MINERALS;
	waterAvailable += waterReplenishedPerTurn;
	if(waterAvailable > MAX_WATER) waterAvailable = MAX_WATER;
}

void Block::drawInfoBox(ALLEGRO_FONT* font){
	isHoveredOn = true;
	al_draw_textf(font, al_map_rgb(25, 200, 100), 25, 200, 0, "Minerals: %i Minerals Per Turn: %i", mineralsAvailable, mineralsReplenishedPerTurn);
}

void Block::draw(ALLEGRO_BITMAP* image){
	if(!isHoveredOn)
		al_draw_bitmap(image, x, y, 0);
	else
		al_draw_tinted_bitmap(image, al_map_rgb(100,100,100), x, y, 0);
	isHoveredOn = false;
}