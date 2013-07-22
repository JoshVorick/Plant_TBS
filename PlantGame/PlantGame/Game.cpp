#pragma once
#include "Game.h"

Game::Game()
{
	mouseX = 0;
	mouseY = 0;
	gameTime = 0;
	turnsTaken = 0;
	font36 = al_load_ttf_font("Fonts/A_Sensible_Armadillo.ttf", 36, 0);
	exitToStart = false;
	camX = 0;
	camY = 0;
	camZ = 8;
	zoom = 1;
}

Game::~Game()
{
	delete map;
	delete options;
	al_destroy_font(font36);
}

void Game::addMap(GameMap* newMap)
{
	map = newMap;
	map->addUnit(1,0,0);
	map->addUnit(0,9,9);
}

void Game::keyPressA()
{
	camX += 100;
}

void Game::keyPressD()
{
	camX -= 100;
}

void Game::keyPressS()
{
	camY -= 100;
}

void Game::keyPressW()
{
	camY += 100;
}

void Game::keyPressPlus()
{
	if(zoom < 2)
		zoom += 0.25;
}

void Game::keyPressMinus()
{
	if(zoom > 0.25)
		zoom -= 0.25;
}

void Game::keyPressEnter()
{
	map->nextTurn();
}

void Game::keyPressSpace()
{
	exitToStart = true;
}

void Game::scroll(int dz)
{
	camZ += dz;			//camZ changes by whatever the mousewheel changed by
	if(camZ < 1)
		camZ = 1;		//These are
	else if(camZ > 8)		//to keep camZ in bounds
		camZ = 8;
	else camY += dz * -32;	//to make it look like the above layers are being added on top when scrolled through
}
//===========================================================================================================
int Game::update()
{
	gameTime++;
	if(exitToStart)
		return 2;
	return -1;
}

void Game::draw()
{
	map->draw(camX,camY,camZ,zoom);	//draws map
	al_draw_text(font36, al_map_rgb(100,0,100), 25,25,0, "Press Space to go back to the start menu");
	al_draw_text(font36, al_map_rgb(100,0,100), 25,60,0, "This is the GAME.");
	al_draw_text(font36, al_map_rgb(100,0,100), 25,95,0, "Use WASD and scroll");
	al_draw_text(font36, al_map_rgb(150,50,150), 25, 130, 0, (map->getPlayers().at(map->getCurPlayerNum())->getName()).c_str());
	//al_draw_textf(font36, al_map_rgb(150,50,150), 25, 130, 0, "%i", map->getCurPlayerNum());
	//draw players
	std::vector<Player*> players = map->getPlayers();
	al_draw_textf(font36, al_map_rgb(255,255,255), windowWidth/2, windowHeight/2-35, ALLEGRO_ALIGN_CENTRE, "%i Player(s)", players.size());

	for(unsigned int i=0; i<players.size(); i++)
		al_draw_textf(font36, al_map_rgb(255,255,0), windowWidth/2, windowHeight/2 + (35*i), ALLEGRO_ALIGN_CENTRE, (char*)(players.at(i)->getName().c_str()));
}