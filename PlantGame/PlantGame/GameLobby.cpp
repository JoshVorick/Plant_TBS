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
	map = new GameMap(10,10,8);
	shouldSendOrReceive = false;
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
				al_draw_textf(font36, al_map_rgb(155,255,155), windowWidth/2, windowHeight/2 + (35*i), ALLEGRO_ALIGN_CENTRE, "%s be a tree!", players.at(i)->getName().c_str());
			else 
				al_draw_textf(font36, al_map_rgb(155,255,155), windowWidth/2, windowHeight/2 + (35*i), ALLEGRO_ALIGN_CENTRE, "%s be a flower!!", players.at(i)->getName().c_str());
	}
}

char* GameLobby::getBitsToBeSent(){
	char* bitsToSend;
	if(map->getPlayers().at(0)->getClass() == 0)
		bitsToSend = "0";
	else
		bitsToSend = "1";
	strcat(bitsToSend, map->getPlayers().at(0)->getName().c_str());

	return bitsToSend;
}

void GameLobby::setBitsReceived(char* bitsReceived){
	std::string str = std::string(bitsReceived);
	Player* newPlayer = new Player();
	newPlayer->setClass(str[0]);
	str.erase(0);
	newPlayer->setName(str);
	map->addPlayer(newPlayer);
}