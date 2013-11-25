#pragma once
#include "GameLobby.h"

GameLobby::GameLobby()
{
	mouseX = 0;
	mouseY = 0;
	windowWidth = 0;
	windowHeight = 0;
	options = NULL;
	font36 = al_load_ttf_font("Fonts/A_Sensible_Armadillo.ttf", 36, 0);
	background = al_load_bitmap("Bitmaps/grass2.png");
	sendbuf = "";
	exitToStart = false;
	startGame = false;
	hasSentType = false;
	hasReceivedType = false;
	shouldSendOrReceive = false;
	map = new GameMap(MAX_X, MAX_Y, MAX_Z);
}

GameLobby::~GameLobby()
{
	delete map;
	map = NULL;
	delete options;
	options  = NULL;
	al_destroy_font(font36);
}
//===========================================================================================================
void GameLobby::keyPressA(){
	if(map != NULL && map->getPlayers().size() == 0){
		Player* newPlayer = new Player();
		newPlayer->setName(playerName);
		newPlayer->setClass(0);
		map->addPlayer(newPlayer);
	}
}

void GameLobby::keyPressD(){
	if(map != NULL && map->getPlayers().size() == 0){
		Player* newPlayer = new Player();
		newPlayer->setName(playerName);
		newPlayer->setClass(1);
		map->addPlayer(newPlayer);
	}
}

void GameLobby::keyPressS()
{

}

void GameLobby::keyPressW()
{

}

void GameLobby::keyPressSpace(){
}

void GameLobby::keyPressEnter(){
	if(map != NULL && map->getNumPlayers() == 2)
		startGame = true;
}

void GameLobby::scroll(int dz)
{

}
//===========================================================================================================
int GameLobby::update()
{
	if(shouldSendOrReceive)
		return 5;
	if(exitToStart)
		return 3;
	if(startGame)
		return 4;
	return -1;
}

void GameLobby::draw(){
	al_draw_bitmap(background, 0, 0, 0);
	al_draw_text(font36, al_map_rgb(255, 255, 55), windowWidth/2, windowHeight/2-200, ALLEGRO_ALIGN_CENTRE, "Welcome to the Game Lobby!");
	al_draw_text(font36, al_map_rgb(255,255,255), windowWidth/2, windowHeight/2-150, ALLEGRO_ALIGN_CENTRE, "Press A to be a tree! Press D to be a flower!");
	//draw player names
	if(map != NULL){
		std::vector<Player*> players = map->getPlayers();
		if(players.size() > 1)
			al_draw_text(font36, al_map_rgb(255,255,255), windowWidth/2, windowHeight/2-115, ALLEGRO_ALIGN_CENTRE, "Press Enter to start the game!");
		if(players.size() == 1)
			al_draw_text(font36, al_map_rgb(255,255,255), windowWidth/2, windowHeight/2-115, ALLEGRO_ALIGN_CENTRE, "Wait for an opponent to join!");
		al_draw_textf(font36, al_map_rgb(55,255,55), windowWidth/2, windowHeight/2-35, ALLEGRO_ALIGN_CENTRE, "%i Player(s):", players.size());
		for(unsigned int i=0; i<players.size(); i++)
			if(players.at(i)->getClass() == 0)
				al_draw_textf(font36, al_map_rgb(155,255,155), windowWidth/2, windowHeight/2 + (35*i), ALLEGRO_ALIGN_CENTRE, "aaa %s be a tree!", players.at(i)->getName().c_str());
			else 
				al_draw_textf(font36, al_map_rgb(155,255,155), windowWidth/2, windowHeight/2 + (35*i), ALLEGRO_ALIGN_CENTRE, "bbb %s be a flower!!", players.at(i)->getName().c_str());
	}
}

struct bitsForSending GameLobby::getBitsToBeSent(){
	struct bitsForSending bitsToSend;

	if(map->getPlayers().at(0)->getClass() == 0)
		bitsToSend.numbers[0] = 0;
	else
		bitsToSend.numbers[0] = 1;

	return bitsToSend;
}

void GameLobby::setBitsReceived(struct bitsForSending *bitsReceived){
	Player *newPlayer = new Player();
	newPlayer->setClass(bitsReceived->numbers[0]);
	map->addPlayer(newPlayer);

	map->getPlayers().at(1)->playerNumber = bitsReceived->numbers[19];
	map->getPlayers().at(0)->playerNumber = bitsReceived->numbers[19]^1;

	map->addUnit(bitsReceived->numbers[19],0,0);
	map->addUnit(bitsReceived->numbers[19]^1,9,9);
}