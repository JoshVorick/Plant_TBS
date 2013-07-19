#pragma once

#include "Seed.h"

Seed::Seed()
{
}

Seed::~Seed()
{
}

bool Seed::doesGerminate()
{
	return false;
}

Block* Seed::getLocation()
{
	return curLocation;
}

void Seed::moveSeedTo(Block *newLoc)
{
	curLocation = newLoc;
}