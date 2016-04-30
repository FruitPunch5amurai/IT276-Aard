
#ifndef __ENTITY_H_
#define __ENTITY_H_
#include <glib.h>
#include "level.h"
#include "sprite.h"
#include "vectors.h"
#include "particle.h"
#include "items.h"


typedef struct EntityData Entity;

enum ScriptType{
	DefeatMonsters
};
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
	Dungeon,
	LockedDoor,
	LitTorch,
	Chest,
	Torch,
	Item,
	UnlockedDoor,
	NoType,
	Door
};
enum EnemyType{
	Chaser,
	Lurker,
	Snatcher,
	None
};
/**
*@brief Struct for Scripts
*/
typedef struct ScriptData Script;
typedef struct ScriptData{
	int count;
	Entity* ent;
	ScriptType type;
	SDL_Rect location;
	int activated;
	char filename[128];
	int (*update)(Script* script);
}Script;

int DefeatFoes(Script* script);
Script* CreateScript(ScriptType type, char *filename,int RoomId,int x, int y,int w,int h);
void RunScript(Script* script);


/**
*@brief Struct for rooms
*/
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
	Script* script;
	GList *Entities;
}Room;
typedef struct ItemRefData ItemRef;

/**
*@brief Struct for EntityBluePrints
*/
typedef struct EntityBluePrintData{
	EntityType entType;
	EnemyType enemyType;
	Vec2D location;
	int frame;
	char filename[128];
	int count;
	int sizex;
	int sizey;
	ItemRef* ref;
}EntityBluePrint;
EntityBluePrint* CreateBluePrint(EntityType entType,EnemyType enemyType,int x, int y,int roomId,
	int frame,char *filename, int sizex,int sizey,ItemRef* ref);
void FreeEntityBluePrints(Room* room);
void FreeBluePrint(Entity* ent);
void FreeEntityBluePrintsAll();

/**
*@brief Struct for Entity
*/
typedef struct EntityData
{
	int		inuse;
	int		id;
	EntityType	whatAmI;		/**< 0 is Aard, 1 is Spirit, 2 is Enemy, 3 is Portal, 4 is Object,5 is Dungeon*/
	int		speed;
	float	stun;
	int		knockback;
	Entity* objectHolding;
	ItemRef* itemHolding;
	SDL_Rect hitBox;
	SDL_Rect atkBox;
	Vec2D	position;
	Vec2D	position2;
	Vec2D	velocity;
	Vec2D	dimensions;
	Vec2D facing;				/**<*/
	Sprite*	sprite;
	Sprite* sprite2;				/**< Used for entities that need an additional sprite*/
	int	currentAnimation;
	int currentFrame;
	SDL_RendererFlip flipped;
	SDL_Rect* camera;
	Entity *timer;
	int immunity;
	int penalty;
	Room *room;
	char dungeonName[128];
	EntityBluePrint* bp;
	
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
	int EnemyHP;
	int nextMove;
	Uint32 temp;
	int moveIndicator;
	//Door
	int doorId;

	void (*update)(struct EntityData *ent);
	void (*think)(struct EntityData *ent);
	void (*touch)(struct EntityData *ent, struct EntityData *other);
	void (*free)(struct EntityData *ent);
	void(*draw)(struct EntityData *ent);
}Entity;

/**
*@brief Struct for Map
*/
typedef struct 
{
	Sprite *tiles;
	int tileW,tileH;
	int w,h;
	int *data;
	int *data2;
	int *data3;
	int *solidLayer;
	//int *specialLayer;
	//int *specialLayerCopy;
	Sprite *specialLayer;
	int hasSpecialLayer;
	int solidTiles[60];
	int numSolidTiles;
	int numOfSpirits;
	Room *rooms;
	int numberOfRooms;
	char name[128];
	int numOfEntities;
}Map;

enum ItemType{
	Lantern,
	Key,
	NONE
};

/**
*@brief Struct for Items
*/
typedef struct ItemData{
	ItemType itemType;		
	Entity* self;
	int itemID;
	int isEquiped;
	void (*useItem)(struct ItemData *item);
	void (*freeItem)(struct ItemData *item);

}Items;

/**
*@brief Struct for Item References
*/
typedef struct ItemRefData{
	ItemType itemType;
	Sprite* sprite;
	Items *item;
	Vec2D pos;
}ItemRef;

/**
*@brief Struct for Inventory Cursor
*/
typedef struct{
	Sprite* sprite;
	GList* ref;
}InventoryCursor;

/**
*@brief Struct for Inventory
*/
typedef struct InventoryData{
	Sprite* sprite;
	Sprite* keySprite;
	GList *inventory;
	int keys;
	//ItemRef inventory[MAX_INVENTORY];
	int display;
	InventoryCursor* cursor;
}Inventory;
//Map and Room Functions
void InitMapList();
void CloseMapList();
Room *CreateRoom(int id,SDL_Rect *boundary,int *linksTo);
void ConnectRooms();
void ClearRoom();
Room *FindRoomWithID(int id);
Map* CreateMap(int tileW, int tileH,int mapWidth,int mapHeight,int numOfRooms);
void FreeMap();
bool Load(char *mapName);
void LoadLayer(int data[],FILE *file);
void LoadSolidTiles(int data[],FILE *file,int NumSolidTiles);
void GenerateSolidLayer(Map* map);
void LoadObjects(FILE *file ,char *buf,int roomdId);
bool IsTileSolid(Map* map,int tile);
bool CheckSolid(Map* map,int x, int y);
bool CheckTile(int* data,int x,int y);
void DrawSpecialLayer(Map* map);
void DrawMap(int layer, int xOffset ,int yOffset,SDL_Texture* texture);
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

//Item Functions
ItemRef* LoadItem(ItemType item);
void UseItem(Items *item);
void ItemLantern(Items *item);
void FreeItem(ItemRef *item);
void UpdateLantern(Entity* ent);
void DrawLantern(Entity* ent);
void DrawInventory(Inventory* i);
void DrawCursor(Inventory* i);
void DrawItemRef(Inventory* i);
Inventory* InitInventory();
void FreeInventory();


#endif 