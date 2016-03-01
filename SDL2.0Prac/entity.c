#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include "graphics.h"
#include "entity.h"

//Study pass by reference
//Study pass by value

#define MAX_ENTITIES		500

Entity* EntityList = NULL;
/**
*@brief Initializes an Entity List
*/
void InitEntityList()
{
	int x;
	EntityList = (Entity*)malloc(sizeof(Entity) * MAX_ENTITIES);
	memset(EntityList,0,sizeof(Entity) * MAX_ENTITIES);	
	atexit(CloseEntityList);
}
/**
*@brief Deallocates the Entity List
*/
void CloseEntityList()
{
	int i;
	if(!EntityList)
	{
		printf("Entity List not initialized");
		return;
	}
	for(i = 0;i<MAX_ENTITIES;i++)
	{
		if(EntityList[i].sprite != NULL)
		{
		FreeSprite(EntityList[i].sprite);
		EntityList[i].inuse--;
		}
	}
	free(EntityList);
	EntityList = NULL;
}
/**
*@brief Goes through the Entity List and calls each of their update functions
*/
void UpdateEntities()
{
	int i;
	for(i =0;i < MAX_ENTITIES;i++)
	{
		if(!EntityList[i].inuse)
		{
			continue;
		}
		if(EntityList[i].update != NULL)
			(*EntityList[i].update)(&EntityList[i]);
	}
}
/**
*@brief Goes through the Entity List and calls each of their think functions
*/	
void ThinkEntities()
{
	int i;
	for(i = 0;i < MAX_ENTITIES;i++)
	{
		if(!EntityList[i].inuse)
		{
			continue;
		}
		if(EntityList[i].think != NULL)
			(*EntityList[i].think)(&EntityList[i]);
	}
}
/**
*@brief Goes through the Entity List and calls each of their draw functions
*/	
void DrawEntities()
{
	int i;
	for(i = 0;i < MAX_ENTITIES;i++)
	{
		if(!EntityList[i].inuse)
		{
			continue;
		}
		if(EntityList[i].draw != NULL)
			(*EntityList[i].draw)(&EntityList[i]);
	}
}
/**
*@brief Draws the entity to the main renderer
*/
void DrawEntity(Entity *ent,int animationNum,int x, int y)
{
	/*
	Vec2D position;
	SDL_Rec camera;
	camera = GetCamera();
	if(!entity_intersect_rect(ent,camera))
	{
		return
	}
	*/


	Animate(ent->sprite->animation[animationNum],ent->sprite->animation[animationNum]->startFrame);
	DrawSprite(ent->sprite,x
		,y,GetCurrentFrame(ent->sprite->animation[animationNum]),
		GetRenderer(),ent->flipped);
}
/**
*@brief Returns a spot in the Entity list thats not in use
*/
Entity* CreateEntity()
{
	int i;
	for(i = 0;i < MAX_ENTITIES;i++)
	{
		if(EntityList[i].inuse)
		{
			continue;
		}
		memset(&EntityList[i],0,sizeof(Entity));
		EntityList[i].inuse = 1;
		EntityList[i].entityListNum = i;
		return &EntityList[i];
		}
	printf("Reached Max number of Entities.");
	return 0;
}
/**
*@brief Frees up the specified entities spot in the EntityList
*/
void FreeEntity(Entity* ent)
{
	if(ent != NULL){
		ent->inuse--;
		FreeSprite(ent->sprite);
		ent = NULL;
	}
}
/**
*@brief Checks to see if entity has stepped on a solid tile
*@param Map poiner, Entity to check against  
*/
bool OverlapsMap(Map *map,Entity *ent)
{

	int x,y,
		x2 = ent->position.x+ent->dimensions.x-1,
		y2 = ent->position.y+ent->dimensions.y-1;
	for(x = ent->position.x; x < x2;x+=map->tileW)
	{
		for( y = ent->position.y; y < y2;y+=map->tileH)
		{
			if(CheckSolid(map,x/map->tileW,y/map->tileH))
				return true;
		}
		if(CheckSolid(map,x/map->tileW,y2/map->tileH))
			return true;
	}
	for(y = ent->position.y;y < y2;y+=map->tileH)
	{
		if(CheckSolid(map,x2/map->tileW,y/map->tileH))
			return true;
	}
		return false;//CheckSolid(map,x2/map->tileW,y2/map->h);
}
/*
Entity* EntityIntersectAll(Entity *a)
{

}
*/
/*
int EntityIntersect(Entity *a, Entity *b)
{



}
*/
/**
*@brief Practice with Function pointers
*/
void UpdateNone(Entity* ent)
{
	printf("Update occured");
}