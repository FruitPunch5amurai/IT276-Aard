#include <math.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <glib.h>
#include "vectors.h"
#include "graphics.h"
#include "spirit.h"
#include "enemy.h"
#include "obj.h"
#include "collision.h"
#include "player.h"
#include "game.h"
#include "light.h"
#include "level.h"

Map *map;
Map *MapList = NULL;
extern Entity* playerEnt;
extern int  MAX_ENTITIES;
extern Entity* EntityList;
GHashTable *GMaps = g_hash_table_new_full(g_str_hash,g_str_equal,NULL,NULL);

/**
*@brief Allocates memory for mapList
*/
void InitMapList()
{
	MapList = (Map*)malloc(sizeof(Map) *20);
	memset(MapList,0,sizeof(Map) * 20);	
	atexit(CloseMapList);
}
/**
*@brief Frees Allocated memory for mapList
*/
void CloseMapList()
{
	int i;
	if(!MapList)
	{
		printf("Entity List not initialized");
		return;
	}
	free(MapList);
	MapList = NULL;
}
/**
*@brief Allocates memory for map
*@param int tileW for Tilewidth , int TileH for Tileheight, int mapWidth for width of map,
*@param int mapHeight for mapHeight
*/
Map* CreateMap(int tileW, int tileH,int mapWidth,int mapHeight,int numOfRooms)
{
	Map* map;
	map = (Map*)malloc(sizeof(Map));
		map->rooms = (Room*)malloc(sizeof(Room)* numOfRooms);
		map->numberOfRooms = numOfRooms;
		map->tileW = tileW;
		map->tileH = tileH;
		map->w = mapWidth;
		map->h = mapHeight;
		map->numSolidTiles = 0;
		map->numOfEntities = 0;
		return map;
		atexit(FreeMap);
}
/**
*@brief Free Allocated memory for current map pointer
*/
void FreeMap()
{
	if(!map)
	{
		printf("Map not initialized");
		return;
	}
	free(map);
	map= NULL;
}
/**
*@brief Loads in a specified .map and loads in the values into map as well as
*@brief assigns a sprite sheet to the map and generates solidLayer of map
*@param A list of chars which is the .map name, the map to load data in, the spriteImage name
*/
bool Load(char *mapName, char *imageName)
{
	int i = 0, j = 0 , k = 0;
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
			fscanf(file,"%s",buf);
			fscanf(file,"%d",&j);
			map = NULL;
      		map = CreateMap(tileW,tileH,mapWidth,mapHeight,j);
			map->hasSpecialLayer = 0;
			//map->EntityList = (Entity*)malloc(sizeof(Entity) * MAX_ENTITIES);
			//memset(map->EntityList,0,sizeof(Entity) * MAX_ENTITIES);	
  			strncpy(map->name,mapName,128);
			map->solidLayer = (int*)malloc(sizeof(int) * map->w*map->h);
			map->data = (int*)malloc(sizeof(int) * map->w*map->h);
			map->data2 = (int*)malloc(sizeof(int) * map->w*map->h);
			map->data3 = (int*)malloc(sizeof(int) * map->w*map->h);
			//map->specialLayer = (int*)malloc(sizeof(int) * map->w*map->h);
			//map->specialLayerCopy = (int*)malloc(sizeof(int) * map->w*map->h);
	
			memset(map->solidLayer,0,sizeof(map->solidLayer));
			memset(map->data,0,sizeof(map->data));
			memset(map->data2,0,sizeof(map->data2));
			memset(map->data3,0,sizeof(map->data3));
			//memset(map->specialLayer,0,sizeof(map->specialLayer));
			//memset(map->specialLayerCopy,0,sizeof(map->specialLayerCopy));
		
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
		if(strcmp(buf,"#SpecialLayer") == 0)
		{
			map->specialLayer = LoadSprite("images/Shroud.png",800,600);
			map->hasSpecialLayer = 1;
			/*
			map->hasSpecialLayer = 1;
			LoadLayer(map->specialLayer,file);
			map->specialLayerCopy[k] = map->specialLayer[k];
			while(k!= map->w * map->h)
			{
				map->specialLayerCopy[k] = map->specialLayer[k];
				k++;
			}*/
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
	ConnectRooms();
	GenerateSolidLayer(map);
	fclose(file);
	/*
	Map *map1; 
		if(g_hash_table_lookup(GMaps,map->name))
		{
			map1 = (Map*)g_hash_table_lookup(GMaps,map->name);
		}
		*/
	map->tiles = LoadSprite(imageName,tileW,tileH);
	g_hash_table_insert(GMaps,mapName,map);
	if(!map->tiles)
	{
	return false;
	}
	return true;
}

void LoadBreakableObjects(FILE *file)
{
	int j = 0,i = 0;
	int frame = 0;
	int type = 0;
	int count = 0;
	char filename[128];
	int w = 0;
	int h = 0;
	int x = 0;
	int y = 0;
	ItemRef* ref = NULL;
	char buf[500];
	fscanf(file,"%s",buf);
	while(strcmp(buf,"#end") != 0){
	fscanf(file,"%d",&frame);
	fscanf(file,"%s",buf);
	fscanf(file,"%s",buf);
	if(strcmp(buf,"Breakable") == 0)
		type = 4;
	else if(strcmp(buf,"LockedDoor") == 0)
		type = 6;
	else if(strcmp(buf,"Chest") == 0){
		type = 8;
		fscanf(file,"%s",buf);
		fscanf(file,"%s",buf);
		if(strcmp(buf,"Key") == 0)
		{
			ref = LoadItem(Key);
		}
	}
	else if(strcmp(buf,"LitTorch")== 0)
	{
		type = LitTorch;
	}
	else if(strcmp(buf,"Torch")== 0)
	{
		type = Torch;
	}
	fscanf(file,"%s",buf);
	fscanf(file,"%s",filename);
	fscanf(file,"%s",buf);
	fscanf(file,"%d",&count);
	fscanf(file,"%s",buf);
	fscanf(file,"%d",&w);
	fscanf(file,"%s",buf);
	fscanf(file,"%d",&h);
	fscanf(file,"%s",buf);
	for(i = 0;i < count;i++){
		fscanf(file,"%d",&x);
		fscanf(file,"%s",buf);
		fscanf(file,"%d",&y);
 		CreateObject(CreateVec2D(x*32,y*32),w,h,type,frame,filename,ref);
		map->numOfEntities++;
	}
	fscanf(file,"%s",buf);
	}
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
void DrawSpecialLayer(Map* map)
{
	int i;
	
	if(map->hasSpecialLayer)
	{
		DrawSprite(map->specialLayer,playerEnt->room->boundary.x,playerEnt->room->boundary.y,0,GetRenderer(),SDL_FLIP_NONE);
		DrawLights();

	}

}
/**
*@brief Draws the portion of the map seen by screen
*@param The map to draw,The layer to draw,int xOffset for Draw , int yOffset for Draw 
*/
void DrawMap(int layer, int xOffset ,int yOffset)
{
	int draw = 1;
	int* drawLayer = NULL;
	if(map != NULL){
	if(layer == 0)
		drawLayer = map->solidLayer;
	if(layer == 1)
		drawLayer = map->data;
	if(layer == 2)
		drawLayer = map->data2;
	if(layer == 3)
		drawLayer = map->data3;

	int startY = yOffset/map->tileH;
	int startX = xOffset/map->tileW;
	int frame;
	
	int columns = (800/map->tileW)+2;
	int rows = (600 / map->tileH) +2; //Screen resolutions

	int y,x,i;
	SDL_Rect r;
	SDL_SetRenderTarget(GetRenderer(),game->mainSceneTexture);
	for( y = startY; y < 600; y++){
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
					//DrawSprite(map->tiles,
					//x*map->tileW,
					// y*map->tileH,32,GetRenderer(),SDL_FLIP_NONE);
				
				}
			}
			else
			{
				DrawSprite(map->tiles,x*map->tileW,y*map->tileH,frame,GetRenderer(),SDL_FLIP_NONE);
			}
			}
	}
	}
}
	SDL_SetRenderTarget(GetRenderer(),NULL);	
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
/**
*@brief Tells me if tile is 0 on .map
*@param pointer to array of tiles, int x, int y
*/
bool CheckTile(int* data,int x,int y)
{
	if(x < 0 || x > map->w || y < 0 || y > map->h)
        return false;
	if(data[y*map->w + x] != 0)
		return false;
	else
		return true;
}
/**
*@brief Spawns Entities on the map from map file
*@param Map* map pointer to map
*@param FILE* file pointer 
*/
void SetUpMap(Map* map,FILE *file)
{
	char buf[500];
	int j,k,l,i,m,a,RoomId,LinksTo[4];
	char filename[128];
	SDL_Rect *roomDimensions = (SDL_Rect*)malloc(sizeof(SDL_Rect));

	while(fscanf(file,"%s",buf) != EOF){
		if(strcmp(buf,"#Objects") == 0)
		{
 			LoadBreakableObjects(file);
		}
		if(strcmp(buf,"#RoomIds") == 0)
		{
			for(i = 0; i < map->numberOfRooms;++i)
			{
				k = 0;
				fscanf(file,"%s",buf);
				fscanf(file,"%d",&RoomId);
				fscanf(file,"%s",buf);
				fscanf(file,"%d",&LinksTo[k]);
				fscanf(file,"%s",buf);
				while(strcmp(buf,",") == 0){
					k++;
					fscanf(file,"%d",&LinksTo[k]);
					fscanf(file,"%s",buf);
				}
				if(k < 3)
				{
					do
					{
						k++;
						LinksTo[k] = NULL;
					}
					while(k < 3);
					
				}
				fscanf(file,"%d",&roomDimensions->w);
				fscanf(file,"%s",buf);
				fscanf(file,"%d",&roomDimensions->h);
				fscanf(file,"%s",buf);
				fscanf(file,"%d",&roomDimensions->x);
				fscanf(file,"%s",buf);
				fscanf(file,"%d",&roomDimensions->y);
				CreateRoom(RoomId,roomDimensions,LinksTo);
			}
		}
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
				map->numOfEntities++;
			}
		}
		if(strcmp(buf,"#Enemies") == 0){
				fscanf(file,"%s",buf);
				while(strcmp(buf,"#end") != 0){
				fscanf(file,"%s",buf);
				if(strcmp(buf,"Snatcher") == 0)
				{

					fscanf(file,"%s",buf);
					fscanf(file,"%d",&j);
					fscanf(file,"%s",buf);

					for(int i = 0;i < j ; i++ ){
						fscanf(file,"%d",&k);
						fscanf(file,"%s",buf);
						fscanf(file,"%d",&l);
					CreateEnemy(k,l,Snatcher);
					map->numOfEntities++;
					}

				fscanf(file,"%s",buf);
				}
				if(strcmp(buf,"Lurker")== 0)
				{

					fscanf(file,"%s",buf);
					fscanf(file,"%d",&j);
					fscanf(file,"%s",buf);

					for(int i = 0;i < j ; i++ ){
						fscanf(file,"%d",&k);
						fscanf(file,"%s",buf);
						fscanf(file,"%d",&l);

					CreateEnemy(k,l,Lurker);
					map->numOfEntities++;
					}
					fscanf(file,"%s",buf);
				}
				if(strcmp(buf,"Chaser")== 0)
				{
					fscanf(file,"%s",buf);
					fscanf(file,"%d",&j);
					fscanf(file,"%s",buf);
					for(int i = 0;i < j ; i++ ){
						fscanf(file,"%d",&k);
						fscanf(file,"%s",buf);
						fscanf(file,"%d",&l);
					CreateEnemy(k,l,Chaser);
					map->numOfEntities++;
					}
				fscanf(file,"%s",buf);
				}

		}
			
		}
		if(strcmp(buf,"#Portal") == 0){
			fscanf(file,"%s",buf);
			fscanf(file,"%d",&j);
			fscanf(file,"%s",buf);
			fscanf(file,"%d", &k);
			CreatePortal(j,k);
			map->numOfEntities++;
		}
		if(strcmp(buf,"#Dungeons") == 0){
			fscanf(file,"%s",buf);
			fscanf(file,"%s",filename);
			fscanf(file,"%s",buf);
			fscanf(file,"%d", &j);
			fscanf(file,"%s",buf);
			fscanf(file,"%d",&k);
			fscanf(file,"%s",buf);
			fscanf(file,"%d", &l);
			fscanf(file,"%s",buf);
			fscanf(file,"%d",&m);
			fscanf(file,"%s",buf);
			fscanf(file,"%d", &i);
			fscanf(file,"%s",buf);
			fscanf(file,"%d",&a);
			CreateDungeonEntrance(l,m,j,k,filename,i,a);
			map->numOfEntities++;
		}




	}
}

/**
*@brief Connects all the rooms of the map
*/
void ConnectRooms()
{
	for(int i = 0; i <= map->numberOfRooms;++i)
	{
		map->rooms[i].north = map->rooms[i].roomIDs[0] == -1 ?&map->rooms[0]
		:	map->rooms[i].roomIDs[0] == 0 ? NULL
		:	&map->rooms[map->rooms[i].roomIDs[0]];
		map->rooms[i].south = map->rooms[i].roomIDs[1] == -1 ? &map->rooms[0]
		:   map->rooms[i].roomIDs[1] == 0 ? NULL
		:	&map->rooms[map->rooms[i].roomIDs[1]];
		map->rooms[i].east = map->rooms[i].roomIDs[2] == -1 ?&map->rooms[0] 
		:	map->rooms[i].roomIDs[2] == 0 ? NULL
		:	&map->rooms[map->rooms[i].roomIDs[2]];
		map->rooms[i].west = map->rooms[i].roomIDs[3] == -1 ?&map->rooms[0] 
		:	map->rooms[i].roomIDs[3] == 0 ? NULL
		:	&map->rooms[map->rooms[i].roomIDs[3]];
	}
}
/**
*@brief Creates and allocates memory for a room
*@param The rooms id
*@param The rooms size
*@param the list of room id's it links to
*/
Room *CreateRoom(int id,SDL_Rect *boundary,int linksTo[])
{
	Room *room;
	room = (Room*)malloc(sizeof(Room));
	for(int i = 0;i < 4;i++)
	{
		room->roomIDs[i] = linksTo[i];
	}
	room->boundary = *boundary;
	room->id = id;
	if(id == -1)
	{
		map->rooms[0] = *room;
	}else
	{
		map->rooms[id] = *room;
	}
	return room;
}
/**
*@brief Locates a room
*@param id of room
*@return pointer to the room
*/
Room *FindRoomWithID(int id)
{
	for(int i = 0; i <= map->numberOfRooms;++i)
	{
		if(map->rooms[i].id == id)
			return &map->rooms[i];
		else
			return NULL;
	}

}
/**
*@brief Creates a dungeon entrance
*@param int x and y for position, int w and h for width and height
*@param the name of the dungeon it leads to
*@param the position to spawn the player when he touches it
*/
Entity* CreateDungeonEntrance(int x,int y,int w, int h,char filename[128],int playerSpawnX,int playerSpawnY)
{
	Entity* dungeon;
	dungeon = CreateEntity();
	dungeon->whatAmI = Dungeon;
	dungeon->sprite = LoadSprite("images/Resources1.png",w,h);
	dungeon->currentAnimation = 0;

	dungeon->sprite->animation[0].startFrame = 4;
	dungeon->sprite->animation[0].currentFrame = 4;
	dungeon->sprite->animation[0].maxFrames = 1;
	dungeon->sprite->animation[0].oscillate = 0;
	dungeon->position.x = x;
	dungeon->position.y = y;
	dungeon->savedPlayerPos.x = playerSpawnX;
	dungeon->savedPlayerPos.y = playerSpawnY;
	dungeon->dimensions.x = w;
	dungeon->dimensions.y = h;
	dungeon->hitBox.w = w;
	dungeon->hitBox.h = h;
	dungeon->hitBox.x= x;
	dungeon->hitBox.y = y;
	strncpy(dungeon->dungeonName,filename,20);
	for(int i =0;i < map->numberOfRooms;++i)
	{
 		if(AABB(dungeon->hitBox,map->rooms[i].boundary))
		{
			dungeon->room = &map->rooms[i];
		}
	}
	dungeon->draw = &DrawObject;
	dungeon->think = NULL;
 	dungeon->free = &FreeEntity;
	dungeon->touch = NULL;
	return dungeon;
}
/**
*@brief Loads a Dungeon into map pointer
*@param filename of the dungeon, Vec2D where to spawn player
*/
void LoadDungeon(char *filename,Vec2D playerSpawn)
{
	char dungeonName[120];
	strncpy(dungeonName,filename,120);
		CloseEntityList();
		CloseLightList();
		InitLightList();
		InitEntityList();
		Load(dungeonName,"images/Resources1.png");
		CreatePlayer(playerSpawn.x,playerSpawn.y);		
}
/**
*@brief Loads in all the entities associated with the current map
*/
void LoadEntities()
{

}
