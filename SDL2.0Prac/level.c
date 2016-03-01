#include <math.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "vectors.h"
#include "graphics.h"
#include "level.h"


/**
*@brief Allocates memory for map
*@param int tileW for Tilewidth , int TileH for Tileheight, int mapWidth for width of map,
*@param int mapHeight for mapHeight
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
	map->numSolidTiles = 0;
	return map;
}
/**
*@brief Loads in a specified .map and loads in the values into map as well as
*@brief assigns a sprite sheet to the map and generates solidLayer of map
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

	
	map->solidLayer = (int*)malloc(sizeof(int) * map->w*map->h);
	map->data = (int*)malloc(sizeof(int) * map->w*map->h);
	map->data2 = (int*)malloc(sizeof(int) * map->w*map->h);
	map->data3 = (int*)malloc(sizeof(int) * map->w*map->h);

	memset(map->solidLayer,0,sizeof(map->solidLayer));
	memset(map->data,0,sizeof(map->data));
	memset(map->data2,0,sizeof(map->data2));
	memset(map->data3,0,sizeof(map->data3));
	
	LoadLayer(map,map->data,file);
	LoadLayer(map,map->data2,file);
	LoadLayer(map,map->data3,file);
	LoadSolidTiles(map,map->solidTiles,file); /**< MUST TO BE LOADED LAST!*/
	GenerateSolidLayer(map);
	//file = NULL;
	fclose(file);

	map->tiles = LoadSprite(imageName,32,32);
	if(!map->tiles)
	{
	return false;
	}
	return true;
	
}
/**
*@brief Loads in Solid tiles into an array used to keep track of them
*@param Takes in a map pointer, an array of ints, and a file pointer
*/
void LoadSolidTiles(Map* map,int data[],FILE *file)
{
	int j = 0,i = 0;
	
	
	while(fscanf(file,"%d",&j)){
	data[i] = j;
	i++;
	map->numSolidTiles+=1;
	printf("%d",data[i]);
	}
}
/**
*@brief Loads in  tiles for layers into an array used to keep track of them
*@param Takes in a map pointer, an array of ints, and a file pointer
*/
void LoadLayer(Map* map,int data[],FILE *file)
{
	int x,y,i = 0,j = 0;

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
*@brief Generates a solid layer(Array) used to keep track of tiles i can interract with
*@param Takes in a map pointer
*/
void GenerateSolidLayer(Map *map)
{
	int i;
	for(i = 0; i < map->w*map->h;i++)
	{
		if(IsTileSolid(map,map->data3[i]))
			map->solidLayer[i] = 1;
        else
			map->solidLayer[i] = 0;
	}
}

/**
*@brief Draws the portion of the map seen by screen
*@param The map to draw,The layer to draw,int xOffset for Draw , int yOffset for Draw 
*/
void DrawMap(Map *map,int layer, int xOffset ,int yOffset)
{
	int* drawLayer = NULL;
	if(layer == 0)
		drawLayer = map->solidLayer;
	if(layer == 1)
		drawLayer = map->data;
	if(layer == 2)
		drawLayer = map->data2;
	if(layer == 3)
		drawLayer = map->data3;

	int startY = yOffset/map->tileH*2;
	int startX = xOffset/map->tileW*2;
	int frame;
	
	int columns = (800/map->tileW)+2;
	int rows = (600 / map->tileH) +2; //Screen resolutions

	int y,x,i;
	SDL_Rect r;
	for( y = startY; y < 600; y++)
		for(x = startX;x < 800; x++)
		{
			frame = -1;

			if(x >= 0 && y >= 0 && x < map->w && y < map->h){
					frame = drawLayer[y*map->w+x]-1;
			}
		if(frame >= 0){
			if(layer == 0)
			{
				if(frame = 1){
					DrawSprite(map->tiles,
					x*map->tileW,
					 y*map->tileH,32,GetRenderer(),SDL_FLIP_NONE);
				
				}
			}
			else
			{
				DrawSprite(map->tiles,x*map->tileW,y*map->tileH,frame,GetRenderer(),SDL_FLIP_NONE);
			}
		}
	}
}
/**
*@brief Tells me if a tile is solid
*@param Map poiner, and a number which indicates a tile in "layer" 
*/
bool IsTileSolid(Map* map,int tile)
{
    for(unsigned int i = 0; i <map->numSolidTiles; i++)
    {
        if(map->solidTiles[i] == tile)
            return true;
    }
    return false;
}
/**
*@brief Tells me if a tile is solid at these coordinates
*@param Map poiner, int x, int y
*/
bool CheckSolid(Map *map,int x, int y)
{
	if(x < 0 || x > map->w || y < 0 || y > map->h)
        return false;
	else{
		return map->solidLayer[y*map->w+ x];
		}
}

