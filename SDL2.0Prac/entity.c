#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "entity.h"
#include <string>

#define MAX_ENTITIES		500

Entity* EntityList;

void InitEntityList()
{
	int x;
	EntityList = (Entity*)malloc(sizeof(Entity) * MAX_ENTITIES);
	memset(EntityList,0,sizeof(Entity) * MAX_ENTITIES);	
}
void CleanupEntityList()
{

}
void UpdateEntities()
{
	int i;
	for(i =0;i < MAX_ENTITIES;i++)
	{
		if(EntityList[i].update != NULL)
			(*EntityList[i].update)(&EntityList[i]);
	}
}
	
void ThinkEntities()
{
	int i;
	for(i = 0;i < MAX_ENTITIES;i++)
	{
		if(EntityList[i].think != NULL)
			(*EntityList[i].update)(&EntityList[i]);
	}
}

void Draw(Entity *ent,int animationNum)
{
	Animate(ent->sprite->animation[animationNum],0);
	DrawSprite(ent->sprite,0,0,GetCurrentFrame(ent->sprite->animation[animationNum]));
}
Entity* CreateEntity()
{
	int i;
	for(i = 0;i < MAX_ENTITIES;i++)
	{
		if(EntityList[i].inuse == 0 )
		{
			EntityList[i].inuse = 1;
			EntityList[i].entityListNum = i;
			return &EntityList[i];
		}
		}
	printf("Reached Max number of Entities.");
	return 0;
}

void FreeEntity()
{

}

void UpdateNone(Entity* ent)
{
	printf("Update occured");
}