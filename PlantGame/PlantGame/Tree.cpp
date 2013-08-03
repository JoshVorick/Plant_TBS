#pragma once
#include "Tree.h"

Tree::Tree(int w, int h)
{
	width = w;
	height = h;
	classID = TREE;

	mineralsToLevelUp = 100;
	waterToLevelUp = 50;
	mineralsToProduceASeed = 100;
	waterToProduceASeed = 50;
	mineralsStored = 0;
	waterStored = 0;

	hitPoints = 100;
	maxHitPoints = 100;
}

Tree::~Tree()
{

}