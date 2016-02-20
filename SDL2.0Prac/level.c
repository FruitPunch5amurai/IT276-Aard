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
Map* CreateMap(int w, int h)
{
	Map* map;
	map = (Map*)malloc(sizeof(Map));
	memset(map,0,sizeof(map));
	map->tileW = w;
	map->tileH = h;
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
while(!feof(file))
	{
		fscanf(file,"%d",&j);
		map->data[i] = j;
		i++;
	}
	file = NULL;
	
	map->tiles = LoadSprite(imageName,32,32);
	if(!map->tiles)
	{
	return false;
	}
	return true;
	
}
/**
*@brief Draws the map to the screen
*@param The map to draw,int w for width , int h for height
*/
void DrawMap(Map *map, int width ,int height)
{
	int x,y,frame;
	for( y = 0; y < height; y++)
		for(x = 0;x < width; x++)
		{
			frame = map->data[y*width + x];
			frame--;
		if(frame >= 0)
			DrawSprite(map->tiles,x*map->tileW,y*map->tileH,frame,GetRenderer(),SDL_FLIP_NONE);
		}
}