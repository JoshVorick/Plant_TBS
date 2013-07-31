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

int Unit::addMinerals(int mineralsToAdd){
	mineralsStored += mineralsToAdd;
	if(mineralsStored >= mineralsToLevelUp){
		mineralsStored -= mineralsToLevelUp;
		seeds += 1;
		level++;
		if(level % 2 && size < 2)
			size++;
	}
	return seeds;
}