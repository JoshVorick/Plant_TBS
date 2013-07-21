#pragma once
//Allegro Headers -- add to as needed
#include <allegro5\allegro.h>
#include <allegro5\allegro5.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>
#include <allegro5\allegro_primitives.h>

//Class Headers
#include "Block.h"
#include "Bush.h"
#include "EvoTree.h"
#include "Flower.h"
#include "StartMenu.h"
#include "Game.h"
#include "GameLobby.h"
#include "GameMap.h"
#include "GameOptions.h"
#include "Player.h"
#include "Seed.h"
#include "Tree.h"
#include "Unit.h"

//Function Prototypes
void processKeyDown(ALLEGRO_EVENT ev, GameState *state);
void changeState(int newID, GameState *state);
const int WIDTH = 1920;
const int HEIGHT = 1080;
//Global Constants and Variables as needed

int main()
{
	//Create local variables
	const int FPS = 60;
	const int NUM_SAMPLES = 8; //number of sounds playing at one time
	int windowWidth = WIDTH; //NOT display width. Width of the actual on screen window. Used to make mouse still work after resizing display
	int windowHeight = HEIGHT; //^same^
	bool done = false; 
	bool redraw = true;		//This will prevent us from drawing things too often (only draw when timer ticks -> set to true if event is timer)
	GameState *curState;
	//Create Allegro variables
	ALLEGRO_DISPLAY *display;
	ALLEGRO_EVENT_QUEUE *event_queue;
	ALLEGRO_TIMER *timer;

	/*
	.ini File stuff
	-Read .ini file -- prob has graphics options and stuff
	-Process .ini file -- this is where we save the data from the .ini to variables in the program
	*/
	if(!al_init()) 
		return -1;

	al_set_new_display_flags(ALLEGRO_RESIZABLE);	//changes based on .ini
	display = al_create_display(WIDTH, HEIGHT);
	al_set_window_position(display,0,0);		//if windowed, then sets position to top left corner
	al_resize_display(display, WIDTH, HEIGHT);	//<-DONT DELETE This makes the mouse owrk how it should and makes it so that
												//the size of the window works how you would expect.

	al_install_keyboard();
	al_install_audio();
	al_init_acodec_addon();
	al_init_primitives_addon();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_mouse();

	/*
	Initialize Allegro components
	load bitmaps, audio, font, etc
	*/
	curState = new StartMenu();	//sets first state to start menu

	timer = al_create_timer(1.0/FPS);
	event_queue = al_create_event_queue();			//create event queue, then register all sources sp ot works
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));	//For some reason this isn't working...hm...
	al_register_event_source(event_queue, al_get_mouse_event_source());

	al_reserve_samples(NUM_SAMPLES);	//reserves space for NUM_SAMPLES of samples to play at one time

	al_start_timer(timer);	//starts right before game loop does to keep timing happy
	//------------------------------------------------------------------------------------------------------------------------------------------------\\
	//=================================================================================================================================================|>
	//------------------------------------------------------------------------------------------------------------------------------------------------//
	while(!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);	//waits for something to happen (timer, keyboard, mouse etc)
		/*
		Game Loop
			-Read input
			-Process input
			-Update internals
			-Render output to screen
		*/
		switch(ev.type){
			case ALLEGRO_EVENT_DISPLAY_CLOSE:	//If the red X is pressed
				done = true;
				break;
			case ALLEGRO_EVENT_TIMER:			//Timer ticks (happens 60 times/sec)
				switch(curState->update()){		//updates game and checks to see if it needs to change state
					case -1:					//If -1 (usually will be), breaks so it doesn't go through all other cases
						break;
					case 1://start GameLobby()
						delete curState;		//prevent memory leakaage, deletes old GameState
						curState = new GameLobby();
						curState->setwindowSize(windowWidth, windowHeight);
						break;
					case 2://go to start menu
						delete curState;
						curState = new StartMenu();
						curState->setwindowSize(windowWidth, windowHeight);
						break;
					case 3://Go from lobby into game
						GameMap* map = curState->getMap();
						delete curState;
						curState = new Game();
						curState->addMap(map);
						curState->setwindowSize(windowWidth, windowHeight);
						//delete tempPlayers somehow?
						break;
				}
				redraw = true;		//redraw goes to true every 1/60th sec, makes rendering smooth
				break;
			case ALLEGRO_EVENT_KEY_DOWN:	//If a key gets pressed down (NOT if a key IS down, only happens when it is first pressed down)
				processKeyDown(ev, curState);
				break;
			case ALLEGRO_EVENT_MOUSE_AXES:
				curState->setMousePos((ev.mouse.x * WIDTH) / windowWidth, (ev.mouse.y * HEIGHT) / windowHeight);
				curState->scroll(ev.mouse.dz);	//dz is the change in scroll wheel position
				break;
			case ALLEGRO_EVENT_DISPLAY_RESIZE:
				windowWidth = ev.display.width;
				windowHeight = ev.display.height;
				curState->setwindowSize(windowWidth, windowHeight);
				break;
		}
		if(redraw & al_is_event_queue_empty(event_queue)){
			curState->draw();
			al_flip_display();
			al_clear_to_color(al_map_rgb(30,30,30));
		}
	}
	//Game loop over, do stuff that needs to get done before exiting
	delete curState;
	al_destroy_timer(timer);				//delete everything
	al_destroy_event_queue(event_queue);
	al_destroy_display(display);
	return 0;
}
//###############################################################################################################
void processKeyDown(ALLEGRO_EVENT ev, GameState *state)
{
	switch(ev.keyboard.keycode){
		case ALLEGRO_KEY_A:		//all this is self explanatory
			state->keyPressA();
			break;
		case ALLEGRO_KEY_W:
			state->keyPressW();
			break;
		case ALLEGRO_KEY_D:
			state->keyPressD();
			break;
		case ALLEGRO_KEY_S:
			state->keyPressS();
			break;
		case ALLEGRO_KEY_SPACE:
			state->keyPressSpace();
			break;
		case ALLEGRO_KEY_ENTER:
			state->keyPressEnter();
			break;
	}
}

void changeState(int newID, GameState *state)
{
	if (newID == 1){	//new state should be an instance of Game
		//delete state;
		GameState *tempState = new Game();
		state = tempState;
	}
}