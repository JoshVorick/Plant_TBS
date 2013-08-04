#pragma once

#include <vector>
#include "GameState.h"

enum startMenuOptions {START_GAME, HELP, EXIT};

class StartMenu : public GameState
{
private:
	bool gotClicked; //Stores value for if an option was clicked
	int selected; //holds value for which option is being hovered over
	int fontHeight;
        
public:
	StartMenu();	//Does initialization, haven't thought through what arguments it might need
	~StartMenu();	//Garbage collection
	
	void keyPressEnter();
	void mouseDown();

	int update();	//updates whatever needs it every sixtieth of a second (including 'time')
	void draw();	//Magical function that draws everything in-game to the screen
};