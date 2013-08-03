#pragma once
#include "StartMenu.h"

StartMenu::StartMenu()
{
	mouseX = 0;
	mouseY = 0;
	fontHeight = 36;
	gotClicked = false;
	selected = -1;;	//controls when to start the game and go into the game
	font36 = al_load_ttf_font("Fonts/A_Sensible_Armadillo.ttf", 36, 0);
}

StartMenu::~StartMenu()
{
	al_destroy_font(font36);
}

void StartMenu::keyPressEnter()
{
	selected = START_GAME;	//testing purposes. Pressing A goes to lobby
	gotClicked = true;
}

void StartMenu::mouseDown(){
	if(selected == START_GAME || selected == EXIT)
		gotClicked = true;
}

void StartMenu::scroll(int dz)
{

}

int StartMenu::update()
{	
	if(gotClicked && selected == START_GAME)
		return 1;
	if(gotClicked && selected == EXIT)
		return 0;

	int y = fontHeight * 1.5;
	if(mouseY < (windowHeight/2 + y) && mouseY > (windowHeight/2 - y)){	//*Mouse is below top option and above bottom option
		int tempY = mouseY;								//So we don't mess up mouseY
		tempY -= (windowHeight/2 - y);					//Moves range of y-values so that lower bound is 0
		selected = ceil((float)(tempY) / fontHeight) - 1;	//The -5 adjusts for slight offset of draw_text 
	}else
		selected = -1;
	return -1;
}

void StartMenu::draw()
{
	al_draw_rectangle(mouseX-5, mouseY-5, mouseX+5, mouseY+5, al_map_rgb(255,0,0), 5);	//testing purposes
	al_draw_text(font36, al_map_rgb(100,0,100), 25,25,0, "Press Space to make game Lobby");
	al_draw_textf(font36, al_map_rgb(100,0,100), 25,60,0, "This is the START MENU.");
	
	int tempY = windowHeight/2 - (fontHeight * 1.5);
	if(selected == START_GAME)
		al_draw_text(font36, al_map_rgb(0,0,255), windowWidth/2, tempY, ALLEGRO_ALIGN_CENTRE, "Play Game");
	else 
		al_draw_text(font36, al_map_rgb(255,0,255), windowWidth/2, tempY, ALLEGRO_ALIGN_CENTRE, "Play Game");
	tempY += fontHeight;
	
	if(selected == HELP)
		al_draw_text(font36, al_map_rgb(0,0,255), windowWidth/2, tempY, ALLEGRO_ALIGN_CENTRE, "Help");
	else 
		al_draw_text(font36, al_map_rgb(255,0,255), windowWidth/2, tempY, ALLEGRO_ALIGN_CENTRE, "Help");
	tempY += fontHeight;
	
	if(selected == EXIT)
		al_draw_text(font36, al_map_rgb(0,0,255), windowWidth/2, tempY, ALLEGRO_ALIGN_CENTRE, "Exit");
	else 
		al_draw_text(font36, al_map_rgb(255,0,255), windowWidth/2, tempY, ALLEGRO_ALIGN_CENTRE, "Exit");
}