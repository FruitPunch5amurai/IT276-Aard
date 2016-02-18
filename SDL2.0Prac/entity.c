#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "graphics.h"
#include "entity.h"
#include <string>

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
	Animate(ent->sprite->animation[animationNum],ent->sprite->animation[animationNum]->startFrame);
	DrawSprite(ent->sprite,x,y,GetCurrentFrame(ent->sprite->animation[animationNum]),GetRenderer(),ent->flipped);
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
*@brief Practice with Function pointers
*/
void UpdateNone(Entity* ent)
{
	printf("Update occured");
}