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
	int w,h;
	int data[500];
	int data2[500];
	int data3[500];

}Map;

Map* CreateMap(int tileW, int tileH,int mapWidth,int mapHeight);
bool Load(char *mapName,Map *map,char *imageName);
void LoadLayer(Map* map,int data[],FILE *file);

void DrawMap(Map *map,int layer ,int xOffset ,int yOffset);
void UpdateMap(Map* map);

#endif