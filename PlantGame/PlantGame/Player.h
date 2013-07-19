#pragma once

#include <string>
#include <vector>
#include "Unit.h"
#include "EvoTree.h"

class Unit;

class Player
{
private:
	std::string playerName;
	int playerNumber;
	int score;
	EvoTree* evoTree;

public:
	Player();	//Takes care of initializing the player
	~Player();	//Garbage collection
	void setName(std::string name);	//Sets the player's name
	std::string getName();	//Returns the player's name
	void addToScore(int points);	//Increases the player's score by the value passed to the function 
	int getScore();	//Returns the player's current score
	EvoTree* getEvoTree();	//Returns the player's evolution tree
};
