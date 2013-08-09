/*
This will server as the buffer between the StartMenu and the game
This will add all players to the game and establish a connection between each player and the host.

INFO PASSED FOR HOST PC:
means to communicate with others
numplayers and their chosen class (tree, bush, etc.)
game map
more?
INFO PASSED FOR OTHER PLAYERS:
means to communicate with host
game map
more?
*/
#pragma once

#include <vector>
#include "Player.h"
#include "GameMap.h"
#include "GameOptions.h"
#include "GameState.h"

class GameLobby : public GameState
{
private:
	bool startGame;
	bool exitToStart;
	bool hasSentType;

public:
	GameLobby();	//Will have to work for host and non-host joining host's lobby
	~GameLobby();
	
	void keyPressA();
	void keyPressD();
	void keyPressS();
	void keyPressW();
	void keyPressSpace();
	void keyPressEnter();
	void scroll(int dz);

	int update();	//updates whatever needs it every sixtieth of a second (including 'time')
	void draw();	//Magical function that draws everything in-game to the screen
	char* getBitsToBeSent();
	void setBitsReceived(char* bitsReceived);
};