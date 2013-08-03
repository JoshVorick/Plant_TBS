#pragma once

#include <vector>
#include "Player.h"
#include "GameMap.h"
#include "GameOptions.h"
#include "GameState.h"
#include "Tree.h"
#include "Flower.h"
#include "Bush.h"

class Game : public GameState
{
private:
	int camX;	//Each player will have their own game class, so these variables can be in the Game class
	int camY;	//argggg, Imma pirate!!
	int camZ;
	double zoom;
	int turnsTaken;	//The number of total turns taken in the game; increments once every time the game cycles through all the players
	int gameTime;	//Maybe this won't be an int, but this will be some variable that holds how long (real-time) the game has been running
    bool exitToStart;

public:
	Game();	//Does initialization, haven't thought through what arguments it might need
	~Game();	//Garbage collection
	
	void addMap(GameMap* newMap); //adds vector of players to the class

	void keyPressA();
	void keyPressD();
	void keyPressS();
	void keyPressW();
	void keyPressPlus();
	void keyPressMinus();
	void keyPressSpace();
	void keyPressEnter();
	void mouseDown();
	void scroll(int dz);
	
	int update();	//updates whatever needs it every sixtieth of a second (including 'time')
	void draw();	//Magical function that draws everything in-game to the screen
};