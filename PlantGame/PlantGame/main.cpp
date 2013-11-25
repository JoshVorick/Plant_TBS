#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

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
#include "Server.h"
#include "Tree.h"
#include "Unit.h"

#pragma comment (lib, "Ws2_32.lib")

//Function Prototypes
void processKeyDown(ALLEGRO_EVENT ev, GameState *state, struct client *myFancySock, ALLEGRO_EVENT_QUEUE *ev_queue);
void changeState(int newID, GameState *state);
void initializeServer(struct client *newServer);
void initializeClient(struct client *newClient);
void connectClient(GameState *curState, struct client *myFancySock);
void connectHost(GameState *curState, struct client *myFancySock);

//Global Constants and Variables as needed
bool readyToConnect = false;
bool amIHosting = false;
bool inGame = false;

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
	
	struct client myFancySock;

	/*
	.ini File stuff
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
	curState->setwindowSize(WIDTH, HEIGHT);

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

		switch(ev.type){
		case ALLEGRO_EVENT_DISPLAY_CLOSE:	//If the red X is pressed
			done = true;
			break;
		case ALLEGRO_EVENT_TIMER:			//Timer ticks (happens 60 times/sec)
			int updateNumber;
			updateNumber = curState->update();
			switch(updateNumber){		//updates game and checks to see if it needs to change state
			case -1:					//If -1 (usually will be), breaks so it doesn't go through all other cases
				break;
			case 0:
				done = true;
				break;
			case 1://start GameLobby as HOST
				{amIHosting = true;
				delete curState;
				curState = new GameLobby();
				curState->setwindowSize(WIDTH, HEIGHT);
				curState->setHost("HOST");
				initializeServer(&myFancySock);
				readyToConnect = true;
				break;}
			case 2: //start GameLobby as CLIENT
				{amIHosting = false;
				delete curState;
				curState = new GameLobby();
				curState->setwindowSize(WIDTH, HEIGHT);
				curState->setHost("CLIENT");
				initializeClient(&myFancySock);
				readyToConnect = true;
				break;}
			case 3://go to start menu
				delete curState;
				curState = new StartMenu();
				curState->setwindowSize(WIDTH, HEIGHT);
				inGame = false;
				break;
			case 4://Go from lobby into game
				{GameMap* map = curState->getMap();
				delete curState;
				curState = new Game();
				curState->addMap(map);
				curState->setwindowSize(WIDTH, HEIGHT);
				inGame = true;
				if(!amIHosting){
					//Receive necessary bits from other player
					myFancySock.iResult = recv(myFancySock.ClientSocket, (char*)(struct bitsForSending *)&myFancySock.recvbuf, sizeof(struct bitsForSending), 0);
					curState->setBitsReceived((struct bitsForSending*)&myFancySock.recvbuf);
					while(!al_is_event_queue_empty(event_queue)){
						al_drop_next_event(event_queue);
					}
				}
				//delete tempPlayers somehow?
				break;}
			case 5: //Client/Server stuff needs to get did
				break;
				//send bits or receive them
			}
			redraw = true;		//redraw goes to true every 1/60th sec, makes rendering smooth
			break;
		case ALLEGRO_EVENT_KEY_DOWN:	//If a key gets pressed down (NOT if a key IS down, only happens when it is first pressed down)
			if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
				done = true;
			processKeyDown(ev, curState, &myFancySock, event_queue);
			break;
		case ALLEGRO_EVENT_MOUSE_AXES:
			curState->setMousePos((ev.mouse.x * WIDTH) / windowWidth, (ev.mouse.y * HEIGHT) / windowHeight);
			curState->scroll(ev.mouse.dz);	//dz is the change in scroll wheel position
			break;
		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
			curState->mouseDown();
			break;
		case ALLEGRO_EVENT_DISPLAY_RESIZE:
			windowWidth = ev.display.width;
			windowHeight = ev.display.height;
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
void processKeyDown(ALLEGRO_EVENT ev, GameState *state, struct client *myFancySock, ALLEGRO_EVENT_QUEUE *ev_queue)
{
	switch(ev.keyboard.keycode){
		case ALLEGRO_KEY_A:		//all this is self explanatory
			state->keyPressA();
			//If it needs to connect, do it right after picking your class
			if(readyToConnect){
				state->draw();
				al_flip_display();
				al_clear_to_color(al_map_rgb(30,30,30));
				if(amIHosting)
					connectHost(state, myFancySock);
				else
					connectClient(state, myFancySock);
				readyToConnect = false;
			}
			break;
		case ALLEGRO_KEY_W:
			state->keyPressW();
			break;
		case ALLEGRO_KEY_D:
			state->keyPressD();
			//If it needs to connect, do it right after picking your class
			if(readyToConnect){
				state->draw();
				al_flip_display();
				al_clear_to_color(al_map_rgb(30,30,30));
				if(amIHosting)
					connectHost(state, myFancySock);
				else
					connectClient(state, myFancySock);
				readyToConnect = false;
			}
			break;
		case ALLEGRO_KEY_S:
			state->keyPressS();
			break;
		case ALLEGRO_KEY_EQUALS:
			state->keyPressPlus();
			break;
		case ALLEGRO_KEY_MINUS:
			state->keyPressMinus();
			break;
		case ALLEGRO_KEY_SPACE:
			state->keyPressSpace();
			if(inGame){
				//Send necessary bits to other player
				myFancySock->sendbuf = state->getBitsToBeSent();
				myFancySock->iResult = send(myFancySock->ClientSocket, (char*)(struct bitsForSending *)&myFancySock->sendbuf, sizeof(struct bitsForSending), 0);
				//draw so you can see the new stuffz
				state->draw();
				al_draw_filled_rectangle(0,0,WIDTH,HEIGHT,al_map_rgba(0,0,0,150));
				al_flip_display();
				al_clear_to_color(al_map_rgb(30,30,30));
				//Receive necessary bits from other player
				myFancySock->iResult = recv(myFancySock->ClientSocket, (char*)(struct bitsForSending *)&myFancySock->recvbuf, sizeof(struct bitsForSending), 0);
				state->setBitsReceived((struct bitsForSending*)&myFancySock->recvbuf);
				while(!al_is_event_queue_empty(ev_queue)){
					al_drop_next_event(ev_queue);
				}
			}
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

void initializeServer(struct client *newServer){
	//Start server and bind socket, but don't listen yet
	newServer->ListenSocket = INVALID_SOCKET;
	newServer->ClientSocket = INVALID_SOCKET;
	newServer->result = NULL;
	newServer->recvbuflen = DEFAULT_BUFLEN;

	newServer->iResult = WSAStartup(MAKEWORD(2,2), &newServer->wsaData);

	ZeroMemory(&newServer->hints, sizeof(newServer->hints));
    newServer->hints.ai_family = AF_INET;
    newServer->hints.ai_socktype = SOCK_STREAM;
    newServer->hints.ai_protocol = IPPROTO_TCP;
    newServer->hints.ai_flags = AI_PASSIVE;

	newServer->iResult = getaddrinfo(NULL, DEFAULT_PORT, &newServer->hints, &newServer->result);
	newServer->ListenSocket = socket(newServer->result->ai_family, newServer->result->ai_socktype, newServer->result->ai_protocol);
	newServer->iResult = bind(newServer->ListenSocket, newServer->result->ai_addr, (int)newServer->result->ai_addrlen);
	
	freeaddrinfo(newServer->result);
}

void initializeClient(struct client *newClient){
	//start client and connect wait to server
	newClient->ClientSocket = INVALID_SOCKET;
	newClient->result = NULL;
	newClient->ptr = NULL;
	newClient->recvbuflen = DEFAULT_BUFLEN;

	newClient->iResult = WSAStartup(MAKEWORD(2,2), &newClient->wsaData);

	ZeroMemory(&newClient->hints, sizeof(newClient->hints));
    newClient->hints.ai_family = AF_UNSPEC;
    newClient->hints.ai_socktype = SOCK_STREAM;
    newClient->hints.ai_protocol = IPPROTO_TCP;
	
	newClient->iResult = getaddrinfo("76.29.52.123", DEFAULT_PORT, &newClient->hints, &newClient->result);
	   // Attempt to connect to an address until one succeeds
    for(newClient->ptr=newClient->result; newClient->ptr != NULL ;newClient->ptr=newClient->ptr->ai_next) {

        // Create a SOCKET for connecting to server
        newClient->ClientSocket = socket(newClient->ptr->ai_family, newClient->ptr->ai_socktype, newClient->ptr->ai_protocol);

        // Connect to server.
        newClient->iResult = connect( newClient->ClientSocket, newClient->ptr->ai_addr, (int)newClient->ptr->ai_addrlen);
        if (newClient->iResult == SOCKET_ERROR) {
            closesocket(newClient->ClientSocket);
            newClient->ClientSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(newClient->result);
}

void connectClient(GameState *curState, struct client *myFancySock){
	//Tell the other person your name and class
	myFancySock->sendbuf = curState->getBitsToBeSent();
	myFancySock->iResult = send(myFancySock->ClientSocket, (char*)(struct bitsForSending *)&myFancySock->sendbuf, sizeof(struct bitsForSending), 0);
	myFancySock->iResult = send(myFancySock->ClientSocket, curState->getMap()->getPlayers().at(0)->getName().c_str(), (int)strlen(curState->getMap()->getPlayers().at(0)->getName().c_str()), 0);
	
	struct bitsForSending mapGettingSent[MAX_Z/2];
	GameMap *tempMap = curState->getMap();
	ALLEGRO_BITMAP* referenceBlock = al_load_bitmap("Bitmaps/Soil4.bmp");
	int blockWidth = al_get_bitmap_width(referenceBlock);
	int blockHeight = al_get_bitmap_height(referenceBlock)-1; //-1 because it looks better;
	int blockPerceivedHeight = 32;
	for(int i=0; i<=MAX_X; i++){
		for(int j=0;j<=MAX_Y; j++){
			for(int k=0; k<MAX_Z/2; k+=2){
				if(tempMap->blockMap[i][j][k] != NULL)
					mapGettingSent[k/2].numLevels[i][j] = tempMap->blockMap[i][j][k]->getBitmap();
				else
					mapGettingSent[k/2].numLevels[i][j] = -1;

				if(tempMap->blockMap[i][j][k+1] != NULL)
					mapGettingSent[k/2].numSeeds[i][j] = tempMap->blockMap[i][j][k+1]->getBitmap();
				else
					mapGettingSent[k/2].numSeeds[i][j] = -1;
			}
		}
	}
	//Initialize Block and Unit coordinates
	for(int i=0; i<MAX_X; i++){	//x
		for(int j=0; j<MAX_Y; j++){ //y{	
			int top=0;
			for(int k=0; k<MAX_Z; k++){//z
				//Initialize block coordinates
				if(tempMap->blockMap[i][j][k] != NULL){
					tempMap->blockMap[i][j][k]->setCoordinates(((MAX_X-1)*blockWidth/2)+(i*blockWidth/2)-(j*blockWidth/2),
						blockPerceivedHeight+((j+i)*(blockHeight-blockPerceivedHeight)/2)-((k)*(blockPerceivedHeight-4)),1);
					top++;
				}
			}
		}
	}
	curState->addMap(tempMap);
	myFancySock->iResult = send(myFancySock->ClientSocket, (char*)(struct bitsForSending *)mapGettingSent, sizeof(struct bitsForSending)*MAX_Z/2, 0);

	//Receive what other person's name and class is
	myFancySock->iResult = recv(myFancySock->ClientSocket, (char*)(struct bitsForSending *)&myFancySock->recvbuf, sizeof(struct bitsForSending), 0);
	myFancySock->recvbuf.numbers[19] = 0;
	curState->setBitsReceived((struct bitsForSending*)&myFancySock->recvbuf);
	
	char tempChar[DEFAULT_BUFLEN];
	myFancySock->iResult = recv(myFancySock->ClientSocket, tempChar, myFancySock->recvbuflen, 0);
	tempChar[myFancySock->iResult] = '\0';
	curState->getMap()->getPlayers().at(1)->setName(tempChar);
}

void connectHost(GameState *curState, struct client *myFancySock){	
	//Listen for a person to connect
	myFancySock->iResult = listen(myFancySock->ListenSocket, 2);
	myFancySock->ClientSocket = accept(myFancySock->ListenSocket, NULL, NULL);
	closesocket(myFancySock->ListenSocket);

	//Receive what other person's name and class is
	myFancySock->iResult = recv(myFancySock->ClientSocket, (char*)(struct bitsForSending *)&myFancySock->recvbuf, sizeof(struct bitsForSending), 0);
	myFancySock->recvbuf.numbers[19] = 1;
	curState->setBitsReceived((struct bitsForSending*)&myFancySock->recvbuf);

	char tempChar[DEFAULT_BUFLEN];
	myFancySock->iResult = recv(myFancySock->ClientSocket, tempChar, myFancySock->recvbuflen, 0);
	//tempChar[myFancySock->iResult] = '\0';
	curState->getMap()->getPlayers().at(1)->setName(tempChar);
	
	struct bitsForSending mapGettingSent[MAX_Z/2];
	myFancySock->iResult = recv(myFancySock->ClientSocket, (char*)(struct bitsForSending *)mapGettingSent, sizeof(struct bitsForSending)*MAX_Z/2, 0);
	GameMap *tempMap = curState->getMap();
	
	ALLEGRO_BITMAP* referenceBlock = al_load_bitmap("Bitmaps/Soil4.bmp");
	int blockWidth = al_get_bitmap_width(referenceBlock);
	int blockHeight = al_get_bitmap_height(referenceBlock)-1; //-1 because it looks better;
	int blockPerceivedHeight = 32;

	for(int i=0; i<=MAX_X; i++){
		for(int j=0;j<=MAX_Y; j++){
			for(int k=0; k<MAX_Z; k+=2){
				delete tempMap->blockMap[i][j][k];
				if(mapGettingSent[k/2].numLevels[i][j] == -1)
					tempMap->blockMap[i][j][k] = NULL;
				else
					tempMap->blockMap[i][j][k] = new Block(mapGettingSent[k/2].numLevels[i][j], blockWidth, blockHeight);

				delete tempMap->blockMap[i][j][k+1];
				if(mapGettingSent[k/2].numSeeds[i][j] == -1)
					tempMap->blockMap[i][j][k+1] = NULL;
				else
					tempMap->blockMap[i][j][k+1] = new Block(mapGettingSent[k/2].numSeeds[i][j], blockWidth, blockHeight);
			}
		}
	}
	for(int i=0; i<MAX_X; i++){	//x
		for(int j=0; j<MAX_Y; j++){ //y{	
			int top=0;
			for(int k=0; k<MAX_Z; k++){//z
				//Initialize block coordinates
				if(tempMap->blockMap[i][j][k] != NULL){
					tempMap->blockMap[i][j][k]->setCoordinates(((MAX_X-1)*blockWidth/2)+(i*blockWidth/2)-(j*blockWidth/2),
						blockPerceivedHeight+((j+i)*(blockHeight-blockPerceivedHeight)/2)-((k)*(blockPerceivedHeight-4)),1);
					top++;
				}
			}
		}
	}
	
	//Tell the other person your name and class
	myFancySock->sendbuf = curState->getBitsToBeSent();
	myFancySock->iResult = send(myFancySock->ClientSocket, (char*)(struct bitsForSending *)&myFancySock->sendbuf, sizeof(struct bitsForSending), 0);
	myFancySock->iResult = send(myFancySock->ClientSocket, curState->getMap()->getPlayers().at(0)->getName().c_str(), (int)strlen(curState->getMap()->getPlayers().at(0)->getName().c_str()), 0);
}