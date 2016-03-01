#ifndef _LEVEL_H_
#define _LEVEL_H_


#include <string>
#include <stdio.h>
#include "sprite.h"

typedef struct 
{
	Sprite *tiles;
	int tileW,tileH;
	int w,h;
	int *data;
	int *data2;
	int *data3;
	int *solidLayer;
	int solidTiles[60];
	int numSolidTiles;

}Map;

Map* CreateMap(int tileW, int tileH,int mapWidth,int mapHeight);
bool Load(char *mapName,Map *map,char *imageName);
void LoadLayer(Map* map,int data[],FILE *file);
void LoadSolidTiles(Map* map,int data[],FILE *file);
void GenerateSolidLayer(Map* map);
bool IsTileSolid(Map* map,int tile);
bool CheckSolid(Map* map,int x, int y);
void DrawMap(Map *map,int layer ,int xOffset ,int yOffset);

#endif