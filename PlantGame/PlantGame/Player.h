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
	int classID;
	EvoTree* evoTree;

public:
	Player();	//Takes care of initializing the player
	~Player();	//Garbage collection
	void setName(std::string name) {playerName = name;}	//Sets the player's name
	void setClass(int classNum) {classID = classNum;}
	int getClass() {return classID;}
	std::string getName() {return playerName;}	//Returns the player's name
	void addToScore(int points) {score += points;}	//Increases the player's score by the value passed to the function 
	int getScore() {return score;}	//Returns the player's current score
	EvoTree* getEvoTree() {return evoTree;}	//Returns the player's evolution tree
};
