#pragma once
#include "Unit.h"

Unit::Unit(){
	//Defualt values:
	level = 1;
	size = 0;
	seeds = 0;

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
			break;
		}
	}
	//Tell the block below that this unit has roots in it
	hasRootsIn.at(0)->addRoots(unitID);
}

int Unit::addMinerals(){
	int mineralsToAdd = 0;
	for(unsigned int i=0; i<hasRootsIn.size(); i++){
		mineralsToAdd += hasRootsIn.at(i)->giveUnitMinerals(unitID);
	}
	mineralsStored += mineralsToAdd;
	if(mineralsStored >= mineralsToLevelUp){
		mineralsStored -= mineralsToLevelUp;
		if(level % 2 && size < 2){
			size++;
			seeds += 1;
		}
		for(int k=MAX_Z; k>=level; k--){
			if(blockMap[xLoc][yLoc][k] != NULL){
				hasRootsIn.push_back(blockMap[xLoc][yLoc][k-level]);
				//Tell the block below that this unit has roots in it
				hasRootsIn.at(level)->addRoots(unitID);
				break;
			}
		}
		level++;
	}
	return seeds;
}