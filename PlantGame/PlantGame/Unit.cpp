#pragma once
#include "Unit.h"

Unit::Unit(){
	//Defualt values:
	level = 1;
	size = 0;

	mineralsToLevelUp = 100;
	waterToLevelUp = 50;
	mineralsToProduceASeed = 100;
	waterToProduceASeed = 50;
	mineralsStored = 0;
	waterStored = 0;

	hitPoints = 100;
	maxHitPoints = 100;
}

Unit::~Unit(){

}

void Unit::setBlockMap(Block* blocks[MAX_X+1][MAX_Y+1][MAX_Z+1], int unitX, int unitY){
	for(int i=0; i<=MAX_X; i++){
		for(int j=0; j<=MAX_Y; j++){
			for(int k=0; k<=MAX_Z; k++){
				blockMap[i][j][k] = blocks[i][j][k];
			}
		}
	}
 	//Tell unit which block it is on
	xLoc = unitX;
	yLoc = unitY;
	//Add the space below to the units list of blocks with its roots
	for(int k=MAX_Z; k>0; k--){
		if(blockMap[xLoc][yLoc][k] != NULL){
			hasRootsIn.push_back(blockMap[xLoc][yLoc][k]);
			blockMap[xLoc][yLoc][k] = NULL; //set this NULL so it doesn't get added again
			//Tell the block below that this unit has roots in it
			hasRootsIn.at(0)->addRoots(unitID);
			break;
		}
	}
}

bool Unit::makeSeed(){
	if(mineralsStored >= mineralsToProduceASeed){
		mineralsStored -= mineralsToProduceASeed;
		mineralsToProduceASeed += 10;
		return true;
	}
	return false;
}

bool Unit::levelUp(){ //returns true if the size changes
	bool doesSizeChange = false;

	if(mineralsStored >= mineralsToLevelUp){
		mineralsStored -= mineralsToLevelUp;
		if(level % 4 == 3 && size < 2){
			size++;
			doesSizeChange = true;
		}
		for(int k=MAX_Z; k>=level; k--){
			if(blockMap[xLoc][yLoc][k-level] != NULL){
				hasRootsIn.push_back(blockMap[xLoc][yLoc][k-level]);
				blockMap[xLoc][yLoc][k-level] = NULL; //This way, this block won't be added again
				//Tell the block below that this unit has roots in it
				hasRootsIn.at(level)->addRoots(unitID);
				hasRootsIn.at(level)->toggleHighlight();
				break;
			}
		}
		level++;
		mineralsToLevelUp += 25;
	}
	return doesSizeChange;
}

bool Unit::makeASeed(){
	if(mineralsStored >= mineralsToProduceASeed){
		mineralsStored -= mineralsToProduceASeed;
		mineralsToProduceASeed += 10;
		return true;
	}
	else
		return false;
}

void Unit::addMinerals(){
	int mineralsToAdd = 0;
	for(unsigned int i=0; i<hasRootsIn.size(); i++){
		mineralsToAdd += hasRootsIn.at(i)->giveUnitMinerals(unitID);
	}
	mineralsStored += mineralsToAdd;
}