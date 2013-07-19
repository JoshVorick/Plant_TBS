#pragma once
#include "GameMap.h"

GameMap::GameMap(int x, int y, int z)
{
	this->x = x;
	this->y = y;
	this->z = z;
	numPlayers = 2;	//Max of 2 players on this map
	for(int i=0; i<x+1; i++)	//x
		for(int j=0; j<y+1; j++) //y
			for(int k=0; k<z+1; k++)
				blockMap[i][j][k] = NULL;
	for(int i=0; i<x; i++)	//x
		for(int j=0; j<y; j++) //y
		{
			for(int k=0; k<(z-1); k++) //z
			{
				blockMap[i][j][k] = new Block(1 + rand() % 3);	//Bottom layers are solid/whole with random soil type
			}
			for(int k=z-1; k<z; k++) //z
			{
				if(rand() % 2 && blockMap[i][j][k-1] != NULL)		//will randomly create some as long as the block below it is soil
					blockMap[i][j][k] = new Block(1 + rand() % 3);
			}
			unitsOnMap[i][j] = NULL;
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

	for(int i=0;i<NUM_UNIT_TYPES;i++)
		for(int j=0;j<NUM_IMAGES_PER_UNIT;j++)
		{
			unitWidths[i][j] = al_get_bitmap_width(unitImages[i][j]);
			unitHeights[i][j] = al_get_bitmap_height(unitImages[i][j]);
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
	if(player == 0)
		unitsOnMap[x][y] = new Tree();
	else
		unitsOnMap[x][y] = new Flower();

	unitsOnMap[x][y]->setOwner(players.at(player));
}

void GameMap::nextTurn(int nextPlayer)
{
	for(int i=0; i<x; i++)
		for(int j=0; j<y; j++)
			if(unitsOnMap[i][j] != NULL && unitsOnMap[i][j]->getOwner() == players.at(nextPlayer))
				unitsOnMap[i][j]->addMinerals();
}
void GameMap::draw(int camX, int camY, int camZ)
{
	//Draws all blocks that are missing a block above or in front of them
	for(int i=0; i<x; i++)	//x
		for(int j=0; j<y; j++) //y
		{	
			int top=0;
			for(int k=0; k<camZ; k++)//z
			{
				if(blockMap[i][j][k] != NULL)
				{
					if(k==camZ-1 || !(blockMap[i+1][j][k] != NULL && blockMap[i][j+1][k] != NULL && blockMap[i][j][k+1] != NULL))
						al_draw_bitmap(blockImages[blockMap[i][j][k]->getBitmap()],
							camX+((x-1)*blockWidth/2)+(i*blockWidth/2)-(j*blockWidth/2),
							camY+(camZ*blockPerceivedHeight)+((j+i)*(blockHeight-blockPerceivedHeight)/2)-((k)*(blockPerceivedHeight-4)),0);
					top++;
				}
			}
			//draw plant on top of top block
			if(unitsOnMap[i][j] != NULL && (top-1 == camZ || blockMap[i][j][top] == NULL))
				al_draw_bitmap(unitImages[unitsOnMap[i][j]->getClass()][unitsOnMap[i][j]->getSize()],
					camX+((x-1)*blockWidth/2)+(i*blockWidth/2)-((j-1)*blockWidth/2)-(unitWidths[unitsOnMap[i][j]->getClass()][unitsOnMap[i][j]->getSize()]/2),
					camY+(camZ*blockPerceivedHeight)+((j+i+2)*(blockHeight-blockPerceivedHeight)/2)-((top)*(blockPerceivedHeight-4))-(unitHeights[unitsOnMap[i][j]->getClass()][unitsOnMap[i][j]->getSize()]),0);
			/*let's you see how it draws
			al_flip_display();
			al_rest(0.1);*/
		}
}