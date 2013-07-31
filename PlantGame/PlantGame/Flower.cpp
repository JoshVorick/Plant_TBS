#pragma once
#include "Flower.h"

Flower::Flower(int w, int h)
{
	width = w;
	height = h;
	classID = FLOWER;

	mineralsToLevelUp = 100;
	waterToLevelUp = 50;
	mineralsToProduceASeed = 100;
	waterToProduceASeed = 50;

	hitPoints = 100;
	maxHitPoints = 100;

}

Flower::~Flower()
{

}