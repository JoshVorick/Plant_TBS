#pragma once

#include "Unit.h"

class Tree : public Unit
{
public:
	Tree(int w, int h);	//Sets classID to Tree plus whatever else we come up with
	~Tree();	//Garbage collection
};
