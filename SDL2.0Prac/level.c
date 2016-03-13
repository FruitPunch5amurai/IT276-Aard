#include <math.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "vectors.h"
#include "graphics.h"
#include"entity.h"
#include"spirit.h"
#include "enemy.h"
#include "level.h"

Map *map;
extern void SetUpMap(Map* map,FILE *file);
/**
*@brief Allocates memory for map
*@param int tileW for Tilewidth , int TileH for Tileheight, int mapWidth for width of map,
*@param int mapHeight for mapHeight
*/
Map* CreateMap(int tileW, int tileH,int mapWidth,int mapHeight)
{
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
bool Load(char *mapName, char *imageName)
{
	int i = 0, j = 0;
	int NumSolidTiles = 0;
	int mapWidth,mapHeight,tileH,tileW;
	char buf[500];
	FILE * file;
	file = fopen(mapName,"r");

	if(!file)
	 {
		 printf("Failed to load map");
		 return false;
	 }

	

	while(fscanf(file,"%s",buf) != EOF){
		if(strcmp(buf,"#MapWidth") == 0){
			fscanf(file,"%d",&j);
			mapWidth = j;	
			}
		if(strcmp(buf,"#MapHeight") == 0)
		{
			fscanf(file,"%d",&j);
			mapHeight = j;
		}
		if(strcmp(buf,"#TileWidth") == 0)
		{
			fscanf(file,"%d",&j);
			tileW = j;
		}
		if(strcmp(buf,"#TileHeight") == 0)
		{
			fscanf(file,"%d",&j);
			tileH = j;
			map = CreateMap(tileW,tileH,mapWidth,mapHeight);
			map->solidLayer = (int*)malloc(sizeof(int) * map->w*map->h);
			map->data = (int*)malloc(sizeof(int) * map->w*map->h);
			map->data2 = (int*)malloc(sizeof(int) * map->w*map->h);
			map->data3 = (int*)malloc(sizeof(int) * map->w*map->h);
	
			memset(map->solidLayer,0,sizeof(map->solidLayer));
			memset(map->data,0,sizeof(map->data));
			memset(map->data2,0,sizeof(map->data2));
			memset(map->data3,0,sizeof(map->data3));
		}
		if(strcmp(buf,"#NumSolidTiles") == 0)
		{
			fscanf(file,"%d",&j);
			NumSolidTiles = j;
			map->numSolidTiles = j;
		}
		if(strcmp(buf,"#Layer1") == 0)
		{
		LoadLayer(map->data,file);
		}
		if(strcmp(buf,"#Layer2") == 0)
		{
		LoadLayer(map->data2,file);
		}
		if(strcmp(buf,"#Layer3") == 0)
		{
		LoadLayer(map->data3,file);
		}
		if(strcmp(buf,"#SolidTiles") == 0)
		{
		LoadSolidTiles(map->solidTiles,file,NumSolidTiles);
		}
		if(strcmp(buf,"#MapEntities") == 0)
		{
			SetUpMap(map,file);
		}
	}
	GenerateSolidLayer(map);
	fclose(file);

	map->tiles = LoadSprite(imageName,tileW,tileH);
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
void LoadSolidTiles(int data[],FILE *file,int NumSolidTiles)
{
	int j = 0,i = 0;
	char buf[500];
	for(i = 0; i < NumSolidTiles;i++)
	{
		fscanf(file,"%d",&j);
		data[i] = j;
		printf("%d",data[i]);
	}
	
}
/**
*@brief Loads in  tiles for layers into an array used to keep track of them
*@param Takes in a map pointer, an array of ints, and a file pointer
*/
void LoadLayer(int data[],FILE *file)
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
void DrawMap(int layer, int xOffset ,int yOffset)
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
void SetUpMap(Map* map,FILE *file)
{
	char buf[500];
	int j,k,l,i;
	while(fscanf(file,"%s",buf) != EOF){
		if(strcmp(buf,"#Spirits") == 0){
			fscanf(file,"%s",buf);
			fscanf(file,"%d",&j);
			map->numOfSpirits = j;
			fscanf(file,"%s",buf);
			for(i = 0;i < map->numOfSpirits;i++)
			{
				fscanf(file,"%d",&j);
				fscanf(file,"%s",buf);
				fscanf(file,"%d", &k);
				CreateSpirit(j,k);
			}
		}
		if(strcmp(buf,"#Enemies") == 0){
			fscanf(file,"%s",buf);
			fscanf(file,"%d",&j); 
			for(i = 0; i < j; i++)
			{
				fscanf(file,"%s",buf);
				fscanf(file,"%s",buf);
				if(strcmp(buf,"Snatcher") == 0)
				{
					fscanf(file,"%s",buf);
					fscanf(file,"%d",&k);
					fscanf(file,"%s",buf);
					fscanf(file,"%d",&l);
					CreateEnemy(k,l,2);
				}
				if(strcmp(buf,"Lurker")== 0)
				{
					fscanf(file,"%s",buf);
					fscanf(file,"%d",&k);
					fscanf(file,"%s",buf);
					fscanf(file,"%d",&l);
					CreateEnemy(k,l,1);
				}
				if(strcmp(buf,"Chaser")== 0)
				{
					fscanf(file,"%s",buf);
					fscanf(file,"%d",&k);
					fscanf(file,"%s",buf);
					fscanf(file,"%d",&l);
					CreateEnemy(k,l,0);
				}
			}
		}
		if(strcmp(buf,"#Portal") == 0){
			fscanf(file,"%s",buf);
			fscanf(file,"%d",&j);
			fscanf(file,"%s",buf);
			fscanf(file,"%d", &k);
			CreatePortal(j,k);
		}

	}

}




