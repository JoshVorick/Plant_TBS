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
}

void Game::keyPressA()
{
	camX += 100;
	map->changeCamera(100, 0, 0, 0);
}

void Game::keyPressD()
{
	camX -= 100;
	map->changeCamera(-100, 0, 0, 0);
}

void Game::keyPressS()
{
	camY -= 100;
	map->changeCamera(0, -100, 0, 0);
}

void Game::keyPressW()
{
	camY += 100;
	map->changeCamera(0, 100, 0, 0);
}

void Game::keyPressPlus()
{
	if(zoom < 2)
		zoom += 0.25;
	//map->changeCamera(0, 0, 0, 0.25);
}

void Game::keyPressMinus()
{
	if(zoom > 0.25)
		zoom -= 0.25;
	//map->changeCamera(0, 0, 0, -0.25);
}

void Game::keyPressEnter()
{
	if(gameTime > 60)
		exitToStart = true;
}

void Game::keyPressSpace()
{
	map->nextTurn();
}

void Game::mouseDown(){
	map->mouseClick(mouseX, mouseY);
}

void Game::scroll(int dz)
{
	camZ += dz;			//camZ changes by whatever the mousewheel changed by
	if(camZ < 1)
		camZ = 1;		//These are
	else if(camZ > 8)		//to keep camZ in bounds
		camZ = 8;
	else{
		camY += dz * -32;	//to make it look like the above layers are being added on top when scrolled through
		map->changeCamera(0, 0, dz, 0);
	}
}
//===========================================================================================================
int Game::update()
{
	gameTime++;
	if(exitToStart)
		return 3;
	return -1;
}

void Game::draw()
{
	map->draw(camX,camY,camZ,zoom,font36, mouseX, mouseY);	//draws map
	al_draw_text(font36, al_map_rgb(0,255,0), 25, 25, 0, "Use SPACE to cycle turns");
	al_draw_text(font36, al_map_rgb(0,255,0), 25, 60, 0, "Press ENTER to go to start menu");
	al_draw_text(font36, al_map_rgb(50,150,50), 25,95,0, "Use WASD and scroll");
	al_draw_text(font36, al_map_rgb(60,220,60), 25, 130, 0, (map->getPlayers().at(map->getCurPlayerNum())->getName()).c_str());
}

struct bitsForSending Game::getBitsToBeSent(){
	return map->bitsToSend;
}

void Game::setBitsReceived(struct bitsForSending *bitsReceived){
	map->setBits(bitsReceived);
}