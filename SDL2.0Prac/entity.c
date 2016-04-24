#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include "collision.h"
#include "graphics.h"
#include "game.h"
#include "entity.h"

void ThinkObject(Entity* ent);
void TouchObject(Entity* ent,Entity* other);
void DrawObject(Entity* ent);
void UpdateObject(Entity* ent);
int MAX_ENTITIES	= 500;

extern Entity* playerEnt;
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
		if(EntityList[i].free != NULL)
			(*EntityList[i].free)(&EntityList[i]);
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
		if(EntityList[i].inuse <= 0)
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
	
	SDL_SetRenderTarget(GetRenderer(),game->mainSceneTexture);
	for(i = 0;i < MAX_ENTITIES;i++)
	{
		if(EntityList[i].inuse <= 0 || EntityList[i].sprite == NULL)
		{
			continue;
		}
		//Draw Only what the Camera can see
		if(EntityList[i].position.x+ (EntityList[i].sprite->w/2)   - game->camera->x > 0 && 
			EntityList[i].position.x + (EntityList[i].sprite->w/2) < game->camera->x + game->camera->w
			&& EntityList[i].position.y + (EntityList[i].sprite->h/2)  - game->camera->y > 0 
			&& EntityList[i].position.y + (EntityList[i].sprite->h/2) < game->camera->y + game->camera->h 
			
			)
			{
			if(EntityList[i].draw != NULL)
				(*EntityList[i].draw)(&EntityList[i]);
			}
	}
	SDL_SetRenderTarget(GetRenderer(),NULL);
}
/**
*@brief Draws the entity to the main renderer
*/
void DrawEntity(Entity *ent,int animationNum,int x, int y)
{
	/*
	Vec2D position;
	SDL_Rec game->camera;
	game->camera = GetCamera();
	if(!entity_intersect_rect(ent,game->camera))
	{
		return
	}
	*/
	Animate(&ent->sprite->animation[animationNum],ent->sprite->animation[animationNum].startFrame);
	DrawSprite(ent->sprite,x
		,y,GetCurrentFrame(&ent->sprite->animation[animationNum]),
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
		EntityList[i].id = i;
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
		ent->id =  NULL;
		ent->update = NULL;
		ent = NULL;
	}
}
/**
*@brief Checks to see if entity has stepped on a solid tile
*@param Map poiner, Entity to check against  
*/
Vec2D OverlapsMap(Map *map,Entity *ent)
{
	Vec2D dir;
	Vec2D mid = CreateVec2D(ent->position.x + ent->dimensions.x/2,ent->position.y + ent->dimensions.y/2);
	dir.x = dir.y = 0;
	int x,y,
		x2 = ent->position.x+ent->dimensions.x-1,
		y2 = ent->position.y+ent->dimensions.y-1;
	for(x = ent->position.x; x < x2;x+=map->tileW)
	{
		for( y = ent->position.y; y < y2;y+=map->tileH)
		{
			if(CheckSolid(map,x/map->tileW,y/map->tileH)){
				if(CheckSolid(map,x/map->tileW,y2/map->tileH)){
					dir.x = ent->speed;
				}
				else if(CheckTile(map->data,x/map->tileW,y2/map->tileH))
				{
					//printf("I fell down a hole!");

					
				}
				dir.y = ent->speed;
				return dir;
			}else if(CheckTile(map->data,x/map->tileW,y/map->tileH))
			{
				//printf("I fell down a hole!");

			}
		}
		if(CheckSolid(map,x/map->tileW,y2/map->tileH)){
			dir.y = -ent->speed;
			return dir;
		}else if(CheckTile(map->data,x/map->tileW,y2/map->tileH))
			{
				//printf("I fell down a hole!");

			}
	}
	for(y = ent->position.y;y < y2;y+=map->tileH)
	{
		if(CheckSolid(map,x2/map->tileW,y/map->tileH))
		{
			dir.x = -ent->speed;
			dir.y = ent->speed;
			return dir;
		}else if(CheckTile(map->data,x2/map->tileW,y/map->tileH))
		{
			//printf("I fell down a hole!");

		}
	}
		if(CheckSolid(map,x2/map->tileW,y2/map->tileH))
		{
			dir.x = -ent->speed;
			dir.y = -ent->speed;
			return dir;
		}else if(CheckTile(map->data,x2/map->tileW,y2/map->tileH))
		{
			//printf("I fell down a hole!");

		}
		return dir;
}
/*
*@breif Check for collisions between entities
*@param Entity to check collisions
*@return Entity that is colliding with param entity
*/
Entity* EntityIntersectAll(Entity *a)
{
	int i,j;
	for(i = 0;i < MAX_ENTITIES;i++)
		if(EntityList[i].inuse && &EntityList[i] != a)
		{
			if(a->room->id == playerEnt->room->id)
			{
			if(AABB(a->hitBox,EntityList[i].hitBox)){
				(a->touch)(a,&EntityList[i]);
				return &EntityList[i];
			}
		}
	}
}

/*
*@breif Uses Minkowski's sum do determine side of collision
*@param 2 Entities
*@return A Vec2D
*/
Vec2D CollisionObject(Entity* ent1,Entity* ent2)
{
	Vec2D dir;
	int w,h,dx,dy,wy,hx;
	dir.x =dir.y = 0;
	w = .5*(ent1->hitBox.w + ent2->hitBox.w);
	h = .5*(ent1->hitBox.h + ent2->hitBox.h);
	dx = GetCenter(ent1).x - GetCenter(ent2).x;
	dy = GetCenter(ent1).y - GetCenter(ent2).y;
	wy = w *dy;
	hx = h*dx;
	
			if(wy > hx)
				if(wy > -hx){
					//Top Collision
					dir.y = -ent2->speed;
				}
				else{
					//Left Collision
					dir.x = ent2->speed;
				}
			else
				if(wy > -hx)
				{
					//Right Collision
					dir.x = -ent2->speed;
				}
				else{
					//Bottom Collisions
					dir.y = ent2->speed;
				}
				return dir;
			}


Entity* AttackBoxIntersectAll(Entity *a)
{
	int i,j;
	for(i = 0;i < MAX_ENTITIES;i++)
		if(EntityList[i].inuse && &EntityList[i] != a)
		{
			if(AABB(a->atkBox,EntityList[i].hitBox)){
				(a->touch)(a,&EntityList[i]);
				return &EntityList[i];
			}
		}
}
int GetID(Entity *ent)
{
	return ent->id;
}

Entity* GetEntityByID(int id)
{
	int i;
	for (i = 0;i < MAX_ENTITIES;i++)
	{
		if(!EntityList[i].id == id)
			continue;
	}
	return &EntityList[i];
}
SDL_Rect GetCenter(Entity* ent)
{
	SDL_Rect newBox;
	newBox.x = ent->hitBox.x + ent->hitBox.w/2;
	newBox.y = ent->hitBox.y + ent->hitBox.h/2;
	return newBox;
}

Entity *CreatePortal(int x, int y)
{
	Entity *portal;
	portal = CreateEntity();
 	portal->sprite = LoadSprite("images/Portal.png",32,32);
	portal->whatAmI = Portal;

	portal->position.x =x;
	portal->position.y =y;
	portal->dimensions.x = portal->sprite->w;
	portal->dimensions.y = portal->sprite->h;		
	portal->hitBox.x =x;
	portal->hitBox.y =y;
	portal->hitBox.w = portal->sprite->w;
	portal->hitBox.h = portal->sprite->h;

	portal->currentAnimation = 0;
	portal->sprite->animation[0].oscillate = true ;
	portal->sprite->animation[0].startFrame = 0;
	portal->sprite->animation[0].maxFrames = 3;

	portal->draw = &DrawPortal;
	return portal;
}
void DrawPortal(Entity *ent)
{
	DrawEntity(ent,ent->currentAnimation,ent->position.x,ent->position.y);
}

