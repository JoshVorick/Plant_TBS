#pragma once

#include <vector>
#include "GameState.h"

class StartMenu : public GameState
{
private:
	bool makeLobby;
        
public:
	StartMenu();	//Does initialization, haven't thought through what arguments it might need
	~StartMenu();	//Garbage collection
	

	void keyPressA();
	void keyPressD();
	void keyPressS();
	void keyPressW();
	void keyPressSpace();
	void scroll(int dz);

	int update();	//updates whatever needs it every sixtieth of a second (including 'time')
	void draw();	//Magical function that draws everything in-game to the screen
};
