#pragma once
#include "StartMenu.h"

StartMenu::StartMenu()
{
	mouseX = 0;
	mouseY = 0;
	makeLobby = false;	//controls when to start the game and go into the game
	font36 = al_load_ttf_font("Fonts/A_Sensible_Armadillo.ttf", 36, 0);
}

StartMenu::~StartMenu()
{
	al_destroy_font(font36);
}
//===========================================================================================================

//===========================================================================================================
void StartMenu::keyPressA()
{
}

void StartMenu::keyPressD()
{
}

void StartMenu::keyPressS()
{
}

void StartMenu::keyPressW()
{
}

void StartMenu::keyPressSpace()
{
	makeLobby = true;	//testing purposes. Pressing A goes to lobby
}

void StartMenu::scroll(int dz)
{
}
//===========================================================================================================
int StartMenu::update()
{
	if(makeLobby)
		return 1;
	return -1;
}

void StartMenu::draw()
{
	al_draw_rectangle(mouseX-5, mouseY-5, mouseX+5, mouseY+5, al_map_rgb(255,0,0), 5);	//testing purposes
	al_draw_text(font36, al_map_rgb(100,0,100), 25,25,0, "Press Space to make game Lobby");
	al_draw_text(font36, al_map_rgb(100,0,100), 25,60,0, "This is the START MENU.");
}