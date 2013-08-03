#pragma once
#include "GameLobby.h"

GameLobby::GameLobby()
{
	font36 = al_load_ttf_font("Fonts/A_Sensible_Armadillo.ttf", 36, 0);
	exitToStart = false;
	startGame = false;
	map = new GameMap(10,10,8);
}

GameLobby::~GameLobby()
{
	delete map;
	delete options;
	al_destroy_font(font36);
}
//===========================================================================================================
void GameLobby::keyPressA(){
	if(map != NULL){
		Player* newPlayer = new Player();
		newPlayer->setName("YOU");
		newPlayer->setClass(0);
		map->addPlayer(newPlayer);

		newPlayer = new Player();
		newPlayer->setName("ENEMY");
		newPlayer->setClass(1);
		map->addPlayer(newPlayer);
	}
}

void GameLobby::keyPressD(){
	if(map != NULL){
		Player* newPlayer = new Player();
		newPlayer->setName("YOU");
		newPlayer->setClass(1);
		map->addPlayer(newPlayer);

		newPlayer = new Player();
		newPlayer->setName("ENEMY");
		newPlayer->setClass(0);
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
	if(exitToStart)
		return 2;
	if(startGame)
		return 3;
	return -1;
}

void GameLobby::draw(){
	al_draw_text(font36, al_map_rgb(255, 255, 55), windowWidth/2, windowHeight/2-200, ALLEGRO_ALIGN_CENTRE, "Welcome to the Game Lobby!");
	al_draw_text(font36, al_map_rgb(255,255,255), windowWidth/2, windowHeight/2-150, ALLEGRO_ALIGN_CENTRE, "Press A to be a tree! Press D to be a flower!");
	//draw player names
	std::vector<Player*> players = map->getPlayers();
	if(players.size() > 0)
		al_draw_text(font36, al_map_rgb(255,255,255), windowWidth/2, windowHeight/2-115, ALLEGRO_ALIGN_CENTRE, "Press Enter to start the game!");
	al_draw_textf(font36, al_map_rgb(55,255,55), windowWidth/2, windowHeight/2-35, ALLEGRO_ALIGN_CENTRE, "%i Player(s):", players.size());
	for(unsigned int i=0; i<players.size(); i++)
		if(players.at(i)->getClass() == 0)
			al_draw_textf(font36, al_map_rgb(155,255,155), windowWidth/2, windowHeight/2 + (35*i), ALLEGRO_ALIGN_CENTRE, "%s be a tree!", players.at(i)->getName().c_str());
		else 
			al_draw_textf(font36, al_map_rgb(155,255,155), windowWidth/2, windowHeight/2 + (35*i), ALLEGRO_ALIGN_CENTRE, "%s be a flower!!", players.at(i)->getName().c_str());
}