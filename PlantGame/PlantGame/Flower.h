#pragma once

#include "Unit.h"

class Flower : public Unit
{
public:
    Flower(int w, int h);       //Sets classID to FLOWER plus whatever else we come up with
	~Flower();      //Garbage collection
};
