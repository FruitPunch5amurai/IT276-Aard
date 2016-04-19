#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <cmath>
#include "entity.h"
#include "collision.h"
#include "player.h"
#include"graphics.h"
#include "obj.h"

extern Map* map;
extern Entity* playerEnt;
extern Entity* EntityList;
extern Player* playerData;
extern SDL_Texture* mainSceneTexture;
Entity* CreateObject(Vec2D position,
	int width, int height, 
	int type, int frame,char* filename, ItemRef *item)
{
	Entity *object;
	object= CreateEntity();
	object->sprite = LoadSprite(filename,width,height);
	object->currentAnimation = 0;
	object->position.x = position.x;
	object->position.y = position.y;
	object->speed = 15;
	object->dimensions.x = width;
	object->dimensions.y = height;
	object->hitBox.w = width;
	object->hitBox.h = height;
	object->hitBox.x = position.x;
	object->hitBox.y = position.y;
	for(int i =0;i < map->numberOfRooms;++i)
	{
		if(AABB(object->hitBox,map->rooms[i].boundary))
		{
			object->room = &map->rooms[i];
		}
	}
	if(type == 4){
		object->whatAmI = BreakableObject;
		object->think = &ThinkObject;
		object->touch = &TouchObject;
		object->draw = &DrawObject;
		object->update = &UpdateObject;
		object->free = &BreakObject;
		object->nextThink = 0;
		object->sprite->animation[0].startFrame = frame;
		object->sprite->animation[0].currentFrame = frame;
		object->sprite->animation[0].maxFrames = 1;
		object->sprite->animation[0].oscillate = 0;
		object->sprite->animation[1].startFrame = 16;
		object->sprite->animation[1].currentFrame = 16;
		object->sprite->animation[1].maxFrames = 7;
		object->sprite->animation[1].oscillate = 1;
	}
	if(type == 6)
	{
		object->whatAmI = LockedDoor;
		object->think = &ThinkObject;
		object->touch = &TouchObject;
		object->draw = &DrawObject;
		object->update = &UpdateObject;
		object->free = &BreakObject;
		object->nextThink = 0;
		object->sprite->animation[0].startFrame = frame;
		object->sprite->animation[0].currentFrame = frame;
		object->sprite->animation[0].maxFrames = 1;
		object->sprite->animation[0].oscillate = 0;
	}
	if(type == 8)
	{
		object ->whatAmI = Chest;
		object->think = &ThinkObject;
		object->touch = &TouchObject;
		object->draw = &DrawObject;
		object->update = &UpdateObject;
		object->free = &BreakObject;
		object->nextThink = 0;
		object->itemHolding = item;
		object->position2 = object->position;
		object->speed = 13;
		object->temp = 0;

		object->sprite->animation[0].startFrame = frame;
		object->sprite->animation[0].currentFrame = frame;
		object->sprite->animation[0].maxFrames = 1;
		object->sprite->animation[0].oscillate = 0;
		object->sprite->animation[1].startFrame = 1;
		object->sprite->animation[1].currentFrame = 1;
		object->sprite->animation[1].maxFrames = 1;
		object->sprite->animation[1].oscillate = 0;
	}
	return object;
}

void DrawObject(Entity* ent)
{
	DrawEntity(ent,ent->currentAnimation,ent->position.x,ent->position.y);
	if(ent->whatAmI == 4){
		if(ent->sprite->animation[1].currentFrame == ent->sprite->animation[1].startFrame + ent->sprite->animation[1].maxFrames-1)
		{
			BreakObject(ent);
		}
	}
}
void ThinkObject(Entity* ent)
{
	ent->room = 
		   ent->position.x > ent->room->boundary.x + ent->room->boundary.w ? ent->room = ent->room->west 
		:  ent->position.x < ent->room->boundary.x ? ent->room = ent->room->east
		:  ent->position.y > ent->room->boundary.y + ent->room->boundary.h ? ent->room = ent->room->south
		:  ent->position.y < ent->room->boundary.y ? ent->room = ent->room->north
		:  ent->room;
	if(ent->whatAmI == 4){
		if(ent->nextThink > SDL_GetTicks())
		{
			ent->hitBox.w = ent->dimensions.x;
			ent->hitBox.h = ent->dimensions.y;
			ent->hitBox.y = ent->position.y;
			ent->hitBox.x = ent->position.x;
			if(OverlapsMap(map,ent).x != 0 || OverlapsMap(map,ent).y != 0)
			{
				//Vec2DAdd(ent->position,ent->position,OverlapsMap(map,ent));
				ent->currentAnimation = 1;
				ent->velocity.x = ent->velocity.y = 0;
			}
		}else if(ent->nextThink < SDL_GetTicks() && ent->temp == 1)
		{
			ent->velocity.x = ent->velocity.y = 0;
			ent->hitBox.y = ent->position.y;
			ent->hitBox.x = ent->position.x;
			ent->temp = 0;
			ent->currentAnimation = 1;
		}
	}else if(ent->whatAmI == 6)
	{


	}
	
	else if(ent->whatAmI == 8)
	{
		if(ent->currentAnimation == 1 && ent->temp == 0)
		{
			ent->nextThink = 1000 + SDL_GetTicks();
			ent->temp = 1;
		}
		if(ent->currentAnimation == 1 && ent->nextThink > SDL_GetTicks()){
			SDL_SetRenderTarget(GetRenderer(),mainSceneTexture);
			DrawSprite(ent->itemHolding->sprite,ent->position.x+10,ent->position2.y-=ent->speed,0,GetRenderer(),SDL_FLIP_NONE);
			ent->speed-=1;
			SDL_SetRenderTarget(GetRenderer(),NULL);
		}else if(ent->currentAnimation == 1 && ent->nextThink < SDL_GetTicks())
		{
			FreeItem(ent->itemHolding);
			ent->itemHolding = NULL;
			ent->think = NULL;
		}
			
	}
	ent->currentFrame = ent->sprite->animation[ent->currentAnimation].currentFrame;
}
void TouchObject(Entity* ent,Entity* other)
{
	if(ent->whatAmI == 4){
		if(ent->velocity.x == 0 && ent->velocity.y == 0 && other->whatAmI != 8)
			Vec2DAdd(other->position,other->position,CollisionObject(ent,other));
		else if(other->whatAmI == 2)
		{
			//Do some damage;
			ent->currentAnimation = 1;
			ent->velocity.x = 0;ent->velocity.y = 0;
		              
		}
	}else
	{
		Vec2DAdd(other->position,other->position,CollisionObject(ent,other));
	}
}
void UpdateObject(Entity* ent)
{
	if(ent->whatAmI == 4){
		playerEnt->objectHolding != ent ? ent->position = ent->position:
				ent->position = CreateVec2D(playerEnt->position.x-20,playerEnt->position.y- 50);
		Vec2DAdd(ent->position,ent->position,ent->velocity);
	}else if(ent->whatAmI == 6)
	{

	}
	EntityIntersectAll(ent);
}
void BreakObject(Entity *ent)
{
	FreeEntity(ent);
	ent= NULL;
}
