#ifndef _LEVEL_H_
#define _LEVEL_H_

enum tileType{
	NONE,
	WALL
};

typedef struct 
{
	Sprite *tiles;
	int tileW,tileH;
	int data[500];

}Map;

Map* CreateMap(int w,int h);
bool Load(char *mapName,Map *map,char *imageName);
void DrawMap(Map *map, int width ,int height);
#endif