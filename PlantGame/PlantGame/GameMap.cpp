#pragma once
#include "GameMap.h"

GameMap::GameMap(int x, int y, int z)
{
	this->x = x;
	this->y = y;
	this->z = z;
	numPlayers = 2;	//Max of 2 players on this map
	curPlayer = 0;

	for(int i=0; i<x+1; i++)	//x
		for(int j=0; j<y+1; j++) //y
			for(int k=0; k<z+1; k++)
				blockMap[i][j][k] = NULL;

	for(int i=0; i<x; i++){	//x
		for(int j=0; j<y; j++) {//y
			//INITIALIZE BLOCKS
			for(int k=0; k<(z-UNEVEN_LAYERS); k++) {//z
				blockMap[i][j][k] = new Block(1 + rand() % 3);	//Bottom layers are solid/whole with random soil type
			}
			for(int k=z-UNEVEN_LAYERS; k<z; k++){ //z
				if(rand() % 2 && blockMap[i][j][k-1] != NULL)		//will randomly create some as long as the block below it is soil
					blockMap[i][j][k] = new Block(1 + rand() % 3);
			}
			//INITIALIZE UNITS
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

	blockImages[0] = al_load_bitmap("Bitmaps/Soil1.bmp");
	blockImages[1] = al_load_bitmap("Bitmaps/Soil2.bmp");
	blockImages[2] = al_load_bitmap("Bitmaps/Soil3.bmp");
	blockImages[3] = al_load_bitmap("Bitmaps/Soil4.bmp");
	for(int i=0;i<NUM_SOIL_TYPES;i++)
		al_convert_mask_to_alpha(blockImages[i], al_map_rgb(255,0,255)); //converts hideous magenta to transparent
	
	unitImages[0][0] = al_load_bitmap("Bitmaps/Tree1.bmp");
	unitImages[0][1] = al_load_bitmap("Bitmaps/Tree2.bmp");
	unitImages[0][2] = al_load_bitmap("Bitmaps/Tree3.bmp");
	unitImages[1][0] = al_load_bitmap("Bitmaps/Flower1.bmp");
	unitImages[1][1] = al_load_bitmap("Bitmaps/Flower2.bmp");
	unitImages[1][2] = al_load_bitmap("Bitmaps/Flower3.bmp");
	for(int i=0;i<NUM_UNIT_TYPES;i++)
		for(int j=0; j<NUM_IMAGES_PER_UNIT; j++)
			al_convert_mask_to_alpha(unitImages[i][j], al_map_rgb(255,0,255)); //converts hideous magenta to transparent

	blockWidth = al_get_bitmap_width(blockImages[0]);
	blockHeight = al_get_bitmap_height(blockImages[0])-1; //-1 because it looks better;
	blockPerceivedHeight = 32;

	for(int i=0;i<NUM_UNIT_TYPES;i++){
		for(int j=0;j<NUM_IMAGES_PER_UNIT;j++){
			unitWidths[i][j] = al_get_bitmap_width(unitImages[i][j]);
			unitHeights[i][j] = al_get_bitmap_height(unitImages[i][j]);
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
	return true;
}

void GameMap::addUnit(int player, int x, int y)	//This should only add initial units (future units will be added by GameMap from seeds
{
	//Get player's settings for their unit and adds it to unitsOnMap[x][y]
	if(players.at(player)->getClass() == 0)
		unitsOnMap[x][y] = new Tree();
	else
		unitsOnMap[x][y] = new Flower();

	unitsOnMap[x][y]->setOwner(player);
}

void GameMap::nextTurn()
{

	//Turn some seeds into units
	for(unsigned int i=0;i<seeds.size(); i++){
		if(!(unitsOnMap[seeds.at(i).xLoc][seeds.at(i).yLoc] != NULL) && seeds.at(i).playerNum == curPlayer && rand() % 4 == 0){	//should check germinate of block seed is on where 'rand()' is
			addUnit(seeds.at(i).playerNum, seeds.at(i).xLoc, seeds.at(i).yLoc);
			//delete the seed because it is now a plant!
		}
	}
	//move seeds
	for(unsigned int i=0; i<seeds.size(); i++){
		if(seeds.at(i).xLoc < 9)
			seeds.at(i).xLoc += rand() % 2;
		if(seeds.at(i).xLoc > 0)
			seeds.at(i).xLoc -= rand() % 2;
		
		if(seeds.at(i).yLoc < 9)
			seeds.at(i).yLoc += rand() % 2;
		if(seeds.at(i).yLoc > 0)
			seeds.at(i).yLoc -= rand() % 2;
	}

	curPlayer = (curPlayer+1) % numPlayers;
	//update units and produce their seeds
	for(int i=0; i<x; i++){
		for(int j=0; j<y; j++){
			if(unitsOnMap[i][j] != NULL && unitsOnMap[i][j]->getOwner() == curPlayer){
				int num = unitsOnMap[i][j]->addMinerals();
				for(int k=0; k<num;k++){
					struct seed newSeed;
					newSeed.playerNum = curPlayer;
					newSeed.xLoc = i;
					newSeed.yLoc = j;
					seeds.push_back(newSeed);
				}
			}
		}
	}
}

void GameMap::draw(int camX, int camY, int camZ, double zoom)
{
	ALLEGRO_DISPLAY* tempDisplay = al_get_current_display();
	ALLEGRO_BITMAP* tempBitmap = al_create_bitmap(1920, 1080);
	al_set_target_bitmap(tempBitmap);
	//Draws all blocks that are missing a block above or in front of them
	for(int i=0; i<x; i++){	//x
		for(int j=0; j<y; j++){ //y{	
			int top=0;
			for(int k=0; k<camZ; k++){//z
				if(blockMap[i][j][k] != NULL){
					if(k==camZ-1 || !(blockMap[i+1][j][k] != NULL && blockMap[i][j+1][k] != NULL && blockMap[i][j][k+1] != NULL)){
						al_draw_bitmap(blockImages[blockMap[i][j][k]->getBitmap()],
							camX+((x-1)*blockWidth/2)+(i*blockWidth/2)-(j*blockWidth/2),
							camY+(camZ*blockPerceivedHeight)+((j+i)*(blockHeight-blockPerceivedHeight)/2)-((k)*(blockPerceivedHeight-4)),0);
						//al_flip_display();
						//al_rest(0.03);
					}
					top++;
				}
			}
			//draw plant on top of top block
			if(unitsOnMap[i][j] != NULL && (top-1 == camZ || blockMap[i][j][top] == NULL)){
				al_draw_bitmap(unitImages[unitsOnMap[i][j]->getClass()][unitsOnMap[i][j]->getSize()],
					camX+((x-1)*blockWidth/2)+(i*blockWidth/2)-((j-1)*blockWidth/2)-(unitWidths[unitsOnMap[i][j]->getClass()][unitsOnMap[i][j]->getSize()]/2),
					camY+(camZ*blockPerceivedHeight)+((j+i+2)*(blockHeight-blockPerceivedHeight)/2)-((top)*(blockPerceivedHeight-4))-(unitHeights[unitsOnMap[i][j]->getClass()][unitsOnMap[i][j]->getSize()]),0);
			}
			//draw seeds on top of block on top of plant
			for(unsigned int k=0; k<seeds.size(); k++){
				if(seeds.at(k).xLoc == i && seeds.at(k).yLoc == j){
					int classID = players.at(seeds.at(k).playerNum)->getClass();
					al_draw_bitmap(seedImages[classID],
						camX+((x-1)*blockWidth/2)+(i*blockWidth/2)-((j-1)*blockWidth/2)-(seedWidths[classID]/2),
						camY+(camZ*blockPerceivedHeight)+((j+i+2)*(blockHeight-blockPerceivedHeight)/2)-((top)*(blockPerceivedHeight-4))-(seedHeights[classID]),0);
				}
			}
			//let's you see how it draws
			//al_flip_display();
			//al_rest(0.03);
		}
	}
	al_set_target_bitmap(al_get_backbuffer(tempDisplay));
	al_draw_scaled_bitmap(tempBitmap, 0, 0, 1920, 1080, 0, 0, 1920*zoom, 1080*zoom, 0);
	al_destroy_bitmap(tempBitmap);
}