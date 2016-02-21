#include <math.h>
#include <string>
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "vectors.h"
#include "sprite.h"
#include "graphics.h"
#include "level.h"

/**
*@brief Allocates memory for map
*@param int w for width , int h for height
*/
Map* CreateMap(int tileW, int tileH,int mapWidth,int mapHeight)
{
	Map* map;
	map = (Map*)malloc(sizeof(Map));
	memset(map,0,sizeof(map));
	map->tileW = tileW;
	map->tileH = tileH;
	map->w = mapWidth;
	map->h = mapHeight;

	return map;
}
/**
*@brief Loads in a specified .map and loads in the values into map as well as
*@brief assigns a sprite sheet to the map
*@param A list of chars which is the .map name, the map to load data in, the spriteImage name
*/
bool Load(char *mapName,Map * map, char *imageName)
{
	int i = 0, j = 0;
	FILE * file;
	file = fopen(mapName,"r");

	if(!file)
	 {
		 printf("Failed to load map");
		 return false;
	 }
	/*
while(!feof(file))
	{
		fscanf(file,"%d",&j);
		map->data[i] = j;
		i++;
	}
	*/
	LoadLayer(map,map->data,file);
	LoadLayer(map,map->data2,file);
	LoadLayer(map,map->data3,file);
	file = NULL;
	

	map->tiles = LoadSprite(imageName,32,32);
	if(!map->tiles)
	{
	return false;
	}
	return true;
	
}

void LoadLayer(Map* map,int data[],FILE *file)
{
	int x,y,i = 0,j = 0;;
	for(y = 0; y < map->h;y++)
	{
		for(x = 0; x < map->w;x++)
		{
			fscanf(file,"%d",&j);
			data[i] = j;
			i++;
		}
	}
}
/**
*@brief Draws the map to the screen
*@param The map to draw,int w for width , int h for height
*/
void DrawMap(Map *map,int layer, int xOffset ,int yOffset)
{
	int startY = yOffset/map->tileH;
	int startX = xOffset/map->tileW;
	int frame;
	
	int columns = (800/map->tileW)+2;
	int rows = (600 / map->tileH) +2; //Screen resolutions

	int y,x;
	for( y = startY; y < startY + rows; y++)
		for(x = startX;x < startX+columns; x++)
		{
			frame = -1;

			if(x >= 0 && y >= 0 && x < map->w && y < map->h){
				if(layer == 1)
					frame = map->data[y*map->w+x]-1;
				if(layer == 2)
					frame = map->data2[y*map->w+x]-1;
				if(layer == 3)
					frame = map->data3[y*map->w+x]-1;
			}
		if(frame >= 0)
			DrawSprite(map->tiles,x*map->tileW - xOffset,y*map->tileH-yOffset,frame,GetRenderer(),SDL_FLIP_NONE);
		}
}

void UpdateMap(Map* map){


}