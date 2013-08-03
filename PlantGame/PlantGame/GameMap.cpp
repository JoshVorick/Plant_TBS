#pragma once
#include "GameMap.h"

GameMap::GameMap(int x, int y, int z)
{
	this->x = x;
	this->y = y;
	this->z = z;
	nextUnitID = 0;
	blockMouseIsOn[0]=0;
	blockMouseIsOn[1]=0;
	blockMouseIsOn[2]=0;
	numPlayers = 2;	//Max of 2 players on this map
	curPlayer = 0;

	for(int i=0; i<x+1; i++)	//x
		for(int j=0; j<y+1; j++) //y
			for(int k=0; k<z+1; k++)
				blockMap[i][j][k] = NULL;

	blockImages[0] = al_load_bitmap("Bitmaps/Soil1.bmp");
	blockImages[1] = al_load_bitmap("Bitmaps/Soil2.bmp");
	blockImages[2] = al_load_bitmap("Bitmaps/Soil3.bmp");
	blockImages[3] = al_load_bitmap("Bitmaps/Soil4.bmp");
	for(int i=0;i<NUM_SOIL_TYPES;i++)
		al_convert_mask_to_alpha(blockImages[i], al_map_rgb(255,0,255)); //converts hideous magenta to transparent

	blockWidth = al_get_bitmap_width(blockImages[0]);
	blockHeight = al_get_bitmap_height(blockImages[0])-1; //-1 because it looks better;

	blockPerceivedHeight = 32;
	for(int i=0; i<x; i++){	//x
		for(int j=0; j<y; j++) {//y
			//INITIALIZE BLOCKS
			for(int k=0; k<(z-UNEVEN_LAYERS); k++) {//z
				blockMap[i][j][k] = new Block(1 + rand() % 3, blockWidth, blockHeight);	//Bottom layers are solid/whole with random soil type
			}
			for(int k=z-UNEVEN_LAYERS; k<z; k++){ //z
				if(rand() % 2 && blockMap[i][j][k-1] != NULL)		//will randomly create some as long as the block below it is soil
					blockMap[i][j][k] = new Block(1 + rand() % 3, blockWidth, blockHeight);
			}
			//INITIALIZE UNITS AS NULL
			unitsOnMap[i][j] = NULL;
		}
	}
	seedImages[0] = al_load_bitmap("Bitmaps/SeedTree.bmp");
	seedImages[1] = al_load_bitmap("Bitmaps/SeedFlower.bmp");

	for(int i=0;i<NUM_UNIT_TYPES;i++){
		al_convert_mask_to_alpha(seedImages[i], al_map_rgb(255,0,255));
		seedWidths[i] = al_get_bitmap_width(seedImages[i]);
		seedHeights[i] = al_get_bitmap_height(seedImages[i]);
	}
	
	unitImages[TREE][0] = al_load_bitmap("Bitmaps/Tree1.bmp");
	unitImages[TREE][1] = al_load_bitmap("Bitmaps/Tree2.bmp");
	unitImages[TREE][2] = al_load_bitmap("Bitmaps/Tree3.bmp");
	unitImages[FLOWER][0] = al_load_bitmap("Bitmaps/Flower1.bmp");
	unitImages[FLOWER][1] = al_load_bitmap("Bitmaps/Flower2.bmp");
	unitImages[FLOWER][2] = al_load_bitmap("Bitmaps/Flower3.bmp");
	for(int i=0;i<NUM_UNIT_TYPES;i++)
		for(int j=0; j<NUM_IMAGES_PER_UNIT; j++)
			al_convert_mask_to_alpha(unitImages[i][j], al_map_rgb(255,0,255)); //converts hideous magenta to transparent


	for(int i=0;i<NUM_UNIT_TYPES;i++){
		for(int j=0;j<NUM_IMAGES_PER_UNIT;j++){
			unitWidths[i][j] = al_get_bitmap_width(unitImages[i][j]);
			unitHeights[i][j] = al_get_bitmap_height(unitImages[i][j]);
		}
	}
	//Initialize Block and Unit coordinates
	for(int i=0; i<x; i++){	//x
		for(int j=0; j<y; j++){ //y{	
			int top=0;
			for(int k=0; k<z; k++){//z
				//Initialize block coordinates
				if(blockMap[i][j][k] != NULL){
					blockMap[i][j][k]->setCoordinates(((x-1)*blockWidth/2)+(i*blockWidth/2)-(j*blockWidth/2),
						blockPerceivedHeight+((j+i)*(blockHeight-blockPerceivedHeight)/2)-((k)*(blockPerceivedHeight-4)),1);
					top++;
				}
			}
			//Initialize unit coordinates
			if(unitsOnMap[i][j] != NULL){
				unitsOnMap[i][j]->setCoordinates(((x-1)*blockWidth/2)+(i*blockWidth/2)-((j-1)*blockWidth/2)-(unitWidths[unitsOnMap[i][j]->getClass()][unitsOnMap[i][j]->getSize()]/2),
					blockPerceivedHeight+((j+i+2)*(blockHeight-blockPerceivedHeight)/2)-((top)*(blockPerceivedHeight-4))-(unitHeights[unitsOnMap[i][j]->getClass()][unitsOnMap[i][j]->getSize()]),1);
			}
		}
	}

}

GameMap::~GameMap()
{

}

bool GameMap::addPlayer(Player* newPlayer)
{
	if(players.size() < numPlayers)
		players.push_back(newPlayer);
	else
		return false;
	if(players.size() == numPlayers){
		//Initialize seed and its coordinates
		for(int i=0; i<x; i++)
			for(int j=0; j<y; j++){			
				int top=0;
				for(int k=0; k<z; k++){
					if(blockMap[i][j][k] != NULL)
						top++;
				}
				for(int k=0; k<MAX_PLAYERS; k++){
					seedsOnMap[i][j][k] = new Seed();
					seedsOnMap[i][j][k]->setCoordinates(((x-1)*blockWidth/2)+(i*blockWidth/2)-((j-1)*blockWidth/2)-(seedWidths[players.at(k)->getClass()]/2),
							blockPerceivedHeight+((j+i+2)*(blockHeight-blockPerceivedHeight)/2)-((top)*(blockPerceivedHeight-4))-(seedHeights[players.at(k)->getClass()]),1);
				}
			}
	}
	return true;
}

void GameMap::addUnit(int player, int xLoc, int yLoc)	//This should only add initial units (future units will be added by GameMap from seeds
{
	//Get player's settings for their unit and adds it to unitsOnMap[x][y]
	if(players.at(player)->getClass() == 0)
		unitsOnMap[xLoc][yLoc] = new Tree(unitWidths[TREE][0], unitHeights[TREE][0]);
	else
		unitsOnMap[xLoc][yLoc] = new Flower(unitWidths[FLOWER][0], unitHeights[FLOWER][0]);

	//Add unit to vector of initialized units and add ID to player's list of units
	units.push_back(unitsOnMap[xLoc][yLoc]);
	unitsOnMap[xLoc][yLoc]->setID(nextUnitID);
	players.at(player)->addUnit(nextUnitID);
	unitsOnMap[xLoc][yLoc]->setOwner(player);

	//SET coordinates for new block
	//First figure out how high top block is
	int top=0;
	for(int k=0; k<z; k++)
		if(blockMap[xLoc][yLoc][k] != NULL)
			top++;
		else
			break;

	//set coordinates based on block below it
	unitsOnMap[xLoc][yLoc]->setCoordinates(blockMap[xLoc][yLoc][top-1]->getX()+(blockWidth/2)-(unitWidths[players.at(player)->getClass()][unitsOnMap[xLoc][yLoc]->getSize()]/2),
		blockMap[xLoc][yLoc][top-1]->getY()+(blockHeight/2)-(unitHeights[players.at(player)->getClass()][unitsOnMap[xLoc][yLoc]->getSize()]), 1);

	unitsOnMap[xLoc][yLoc]->setBlockMap(blockMap, xLoc, yLoc);
	//increment ID so next unit will have a unique one
	nextUnitID++;
}

void GameMap::changeCamera(int dx, int dy, int dz, double dZoom){
	for(int i=0; i<x; i++){	//x
		for(int j=0; j<y; j++){ //y{	
			int top=0;
			for(int k=0; k<z; k++){//z
				if(blockMap[i][j][k] != NULL){
					blockMap[i][j][k]->incrementCoordinates(dx, dy, dZoom);
					top++;
				}else
					break;
			}
			if(unitsOnMap[i][j] != NULL)
				unitsOnMap[i][j]->incrementCoordinates(dx, dy, dZoom);
			for(int k=0; k<numPlayers; k++)
				seedsOnMap[i][j][k]->incrementCoordinates(dx, dy, dZoom);
		}
	}
}

void GameMap::nextTurn()
{
	//Turn some seeds into units
	for(int i=0; i<x; i++){
		for(int j=0; j<y; j++){
			//Move the seeds
			if(seedsOnMap[i][j][curPlayer]->hasSeeds()){
				int numSeeds = seedsOnMap[i][j][curPlayer]->getNumSeeds();
				for(int k=0; k<numSeeds; k++){
					seedsOnMap[i][j][curPlayer]->removeASeed();
					int randomNumber = rand() % 5;
					switch(randomNumber){
					case  0:
						if(j<y-1)
							seedsOnMap[i][j+1][curPlayer]->addSeed();
						break;
					case 1:
						if(j>0)
							seedsOnMap[i][j-1][curPlayer]->addSeed();
						break;
					case 2:
						if(i<x-1)
							seedsOnMap[i+1][j][curPlayer]->addSeed();
						break;
					case 3:
						if(i>0)
							seedsOnMap[i-1][j][curPlayer]->addSeed();
						break;
					case 4:
						seedsOnMap[i][j][curPlayer]->addSeed();
						break;
					}
				}
			}
			//update units and add their new seeds to board
			if(unitsOnMap[i][j] != NULL){
				int num = unitsOnMap[i][j]->addMinerals();
				if(num != -1){

					int top=0;
					for(int k=0; k<z; k++){
						if(blockMap[i][j][k] != NULL)
							top++;
						else
							break;
					}
					unitsOnMap[i][j]->setCoordinates(blockMap[i][j][top-1]->getX()+(blockWidth/2)-(unitWidths[players.at(unitsOnMap[i][j]->getOwner())->getClass()][unitsOnMap[i][j]->getSize()]/2),
						blockMap[i][j][top-1]->getY()+(blockHeight/2)-(unitHeights[players.at(unitsOnMap[i][j]->getOwner())->getClass()][unitsOnMap[i][j]->getSize()]), 1);

					//Make a new seeds based on if the unit made more or not
					if(unitsOnMap[i][j]->getOwner() == curPlayer){
						for(int k=0; k<num; k++){
							seedsOnMap[i][j][curPlayer]->addSeed();
						}
					}
				} 		
			}
			//Turn seeds into units (randomly)
			if(!(unitsOnMap[i][j] != NULL) && seedsOnMap[i][j][curPlayer]->hasSeeds() && rand() % 4 == 0){
				addUnit(curPlayer, i, j);
				seedsOnMap[i][j][curPlayer]->removeASeed();
			}
			//Have blocks gain respective resources for this turn
			for(int k=0; k<z; k++){
				if(blockMap[i][j][k] != NULL)
					blockMap[i][j][k]->replenishResources();
			}
		}
	}
	curPlayer = (curPlayer+1) % numPlayers;
}

void GameMap::draw(int camX, int camY, int camZ, double zoom, ALLEGRO_FONT* font, int mouseX, int mouseY)
{
	ALLEGRO_DISPLAY* tempDisplay = al_get_current_display();
	ALLEGRO_BITMAP* tempBitmap = al_create_bitmap(1920/zoom, 1080/zoom);
	al_set_target_bitmap(tempBitmap);
	bool drawUnitText = false;
	//Draws all blocks that are missing a block above or in front of them
	for(int i=0; i<x; i++){	//x
		for(int j=0; j<y; j++){ //y{	
			int top=0;
			for(int k=0; k<camZ; k++){//z
				if(blockMap[i][j][k] != NULL){
					if(k==camZ-1 || !(blockMap[i+1][j][k] != NULL && blockMap[i][j+1][k] != NULL && blockMap[i][j][k+1] != NULL)){
						blockMap[i][j][k]->draw(blockImages[blockMap[i][j][k]->getBitmap()]);
						if(blockMap[i][j][k]->isHoveringOnBlock(mouseX, mouseY)){
							blockMouseIsOn[0]=i;
							blockMouseIsOn[1]=j;
							blockMouseIsOn[2]=k;
							if(unitsOnMap[i][j] != NULL && (top == camZ || blockMap[i][j][top+1] == NULL))
								drawUnitText = true;
							else
								drawUnitText = false;
						}
						//al_flip_display();
						//al_rest(0.03);
					}
					top++;
				}
			}
			//draw plant on top of top block
			if(unitsOnMap[i][j] != NULL && (top-1 == camZ || blockMap[i][j][top] == NULL)){
				unitsOnMap[i][j]->draw(unitImages[unitsOnMap[i][j]->getClass()][unitsOnMap[i][j]->getSize()]);
			}
			//draw seeds on top of block on top of plant
			for(int k=0; k<numPlayers; k++){
				if(seedsOnMap[i][j][k]->hasSeeds() && (top-1 == camZ || blockMap[i][j][top] == NULL))
					seedsOnMap[i][j][k]->draw(seedImages[players.at(k)->getClass()]);
			}
			//let's you see how it draws
			//al_flip_display();
			//al_rest(0.03);
		}
	}
	//draw text for whatever is being hovered on
	al_set_target_bitmap(al_get_backbuffer(tempDisplay));
	al_draw_scaled_bitmap(tempBitmap, 0, 0, 1920/zoom, 1080/zoom, 0, 0, 1920, 1080, 0);
	al_destroy_bitmap(tempBitmap);
	//draw after so it doesn't get scaled with zoom
	blockMap[blockMouseIsOn[0]][blockMouseIsOn[1]][blockMouseIsOn[2]]->drawInfoBox(font);
	if(drawUnitText)
		unitsOnMap[blockMouseIsOn[0]][blockMouseIsOn[1]]->drawInfoBox(font);

}