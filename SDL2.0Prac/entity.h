
#ifndef __ENTITY_H_
#define __ENTITY_H_
#include "level.h"
#include "sprite.h"
#include "vectors.h"
#include "particle.h"
/**
*@brief Core data of the entity system
*/
typedef struct EntityData Entity;
enum EnemyState{
	IDLE,
	MOVING,
	CHASING,
	GOINGUP,
	GOINGDOWN
};

enum GuidedState{
	Untouched,
	BeingGuided,
	Lost,

};
enum EntityType{
	Aard,
	Spirit,
	Enemy,
	Portal,
	BreakableObject,
	Dungeon
};
enum EnemyType{
	Chaser,
	Lurker,
	Snatcher
};
typedef struct RoomData Room;

typedef struct RoomData
{
	Room *north;
	Room *south;
	Room *east;
	Room *west;
	int id;
	int roomIDs[4];
	SDL_Rect boundary;
}Room;
typedef struct EntityData
{
	int		inuse;
	int		id;
	EntityType	whatAmI;		/**< 0 is Aard, 1 is Spirit, 2 is Enemy, 3 is Portal, 4 is Object,5 is Dungeon*/
	int		speed;
	float	stun;
	int		knockback;
	Entity* objectHolding;
	SDL_Rect hitBox;
	SDL_Rect atkBox;
	Vec2D	position;
	Vec2D	position2;
	Vec2D	velocity;
	Vec2D	dimensions;
	Vec2D facing;				/**<*/
	Sprite*	sprite;
	int	currentAnimation;
	int currentFrame;
	SDL_RendererFlip flipped;
	SDL_Rect* camera;
	Entity *timer;
	int immunity;
	int penalty;
	Room *room;
	char dungeonName[128];
	
	//Spirit stuff
	GuidedState spiritState;
	int isBeingGuided;
	Uint32 nextThink;
	Vec2D  savedPlayerPos;
	int spiritIndex;	
	Vec2D offset;
	Entity* follow;
	Particle *particles;
	int value;
	
	//Enemy Stuff
	EnemyState state;			/**< State of enemy*/
	EnemyType enemyType;				/**< 0 for Lurker, 1 for Ghost,2 for Chaser*/
	int nextMove;
	Uint32 temp;
	int moveIndicator;

	void (*update)(struct EntityData *ent);
	void (*think)(struct EntityData *ent);
	void (*touch)(struct EntityData *ent, struct EntityData *other);
	void (*free)(struct EntityData *ent);
	void(*draw)(struct EntityData *ent);
}Entity;

typedef struct 
{
	Entity *EntityList;
	Sprite *tiles;
	int tileW,tileH;
	int w,h;
	int *data;
	int *data2;
	int *data3;
	int *solidLayer;
	int solidTiles[60];
	int numSolidTiles;
	int numOfSpirits;
	Room *rooms;
	int numberOfRooms;
	char name[128];
	int numOfEntities;
}Map;
//Map and Room Functions
void InitMapList();
void CloseMapList();
Room *CreateRoom(int id,SDL_Rect *boundary,int *linksTo);
void ConnectRooms();
Room *FindRoomWithID(int id);
Map* CreateMap(int tileW, int tileH,int mapWidth,int mapHeight,int numOfRooms);
void FreeMap();
bool Load(char *mapName,char *imageName);
void LoadLayer(int data[],FILE *file);
void LoadSolidTiles(int data[],FILE *file,int NumSolidTiles);
void GenerateSolidLayer(Map* map);
void LoadBreakableObjects(int data[],FILE *file);
bool IsTileSolid(Map* map,int tile);
bool CheckSolid(Map* map,int x, int y);
void DrawMap(int layer ,int xOffset ,int yOffset);
void SetUpMap(Map* map,FILE *file);
Entity* CreateDungeonEntrance(int x,int y,int w, int h,char filename[128],int playerSpawnX,int playerSpawnY);
void LoadDungeon(char *filename,Vec2D playerSpawn);
void LoadEntities();
//Entity Functions
void DrawEntity(Entity *ent,int animationNum, int x, int y);	
Entity* CreateEntity();
void FreeEntity(Entity* ent);
void UpdateEntities();
void ThinkEntities();
void DrawEntities(); 
void InitEntityList();
void ClearEntityList();
void CloseEntityList();
Vec2D OverlapsMap(Map *map,Entity *ent);

int EntityIntersect(Entity *a, Entity *b);
Entity* EntityIntersectAll(Entity *a);
Vec2D CollisionObject(Entity* ent1,Entity* ent2);
Entity* AttackBoxIntersectAll(Entity *a);
int GetID(Entity *ent);
Entity* GetEntityByID(int id);
SDL_Rect GetCenter(Entity* ent);

Entity *CreatePortal(int x, int y);
void DrawPortal(Entity *ent);
Entity* CreateTimer(Uint8 time);
void ThinkTimer(Entity *ent);

#endif 