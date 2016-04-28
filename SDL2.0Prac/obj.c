#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <cmath>
#include "entity.h"
#include "collision.h"
#include "player.h"
#include "graphics.h"
#include "game.h"
#include "items.h"
#include "light.h"
#include "obj.h"

extern Map* map;
extern SDL_Texture* LightBuffer;
extern Entity* playerEnt;
extern Entity* EntityList;
extern Player* playerData;

/**
*@brief Creates an entity thats an object
*@param width for width,height for height, type for what type of
*@param object, frame for its current frame, filename for the sprite
*@param file name, and a ref to an item if it contains one
*/
Entity* CreateObject(Vec2D position,
	int width, int height, 
	int type, int frame,char* filename, ItemRef *item,EntityBluePrint* bp)
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
	object->bp = bp;
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
	if(type == 6 || type == 13)
	{
		if(type == 6)
			object->whatAmI = LockedDoor;
		else
			object->whatAmI = Door;
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
	if(type == Torch || type == LitTorch)
	{
		if(type	==	Torch)
			object->whatAmI = Torch;
		else
			object->whatAmI = LitTorch;
		object->sprite2 = LoadSprite("images/lightsource1.png",255,255);
		object->value = 1;
		object->draw = &DrawObject;
		object->update = UpdateObject;
		object->think = &ThinkObject;
		object->free = &BreakObject;
		object->touch = &TouchObject;

		
		object->currentAnimation = 0;
		object->sprite->animation[0].startFrame = frame;
		object->sprite->animation[0].currentFrame = frame;
		object->sprite->animation[0].maxFrames = 1;
		object->sprite->animation[0].oscillate = 0;

		
		object->sprite->animation[1].startFrame = frame+1;
		object->sprite->animation[1].currentFrame = frame+1;
		object->sprite->animation[1].maxFrames = 4;
		object->sprite->animation[1].oscillate = 1;
	}
	return object;
}
/**
*@brief Draw functon for objects
*@param the entity calling it
*/
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
/**
*@brief Think functon for objects
*@param the entity calling it
*/
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
			SDL_SetRenderTarget(GetRenderer(),game->mainSceneTexture);
			DrawSprite(ent->itemHolding->sprite,ent->position.x+10,ent->position2.y-=ent->speed,0,GetRenderer(),SDL_FLIP_NONE);
			ent->speed-=1;
			SDL_SetRenderTarget(GetRenderer(),NULL);
		}else if(ent->currentAnimation == 1 && ent->nextThink < SDL_GetTicks())
		{
			FreeItem(ent->itemHolding);
			ent->itemHolding = NULL;
			ent->think = NULL;
		}
		
	}else if(ent->whatAmI == LitTorch)
	{
		CreateLight(ent);
		ent->currentAnimation = 1;
		ent->think = NULL;
	}
	ent->currentFrame = ent->sprite->animation[ent->currentAnimation].currentFrame;
}
/**
*@brief Toucg functon for objects
*@param the entity calling it and the entity it touched
*/
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
			other->EnemyHP -=5;
		              
		}
	}else if(ent->whatAmI == Torch)
	{
		if(other->whatAmI == Aard && playerData->currentItem->itemType == Lantern)
		{
			ent->whatAmI = LitTorch;
			ent->bp->entType = LitTorch;
		}
	}else if(ent->whatAmI == LockedDoor)
	{
		if(other->whatAmI == Aard){
			if(playerData->inventory->keys > 0)
			{
				ent->whatAmI = UnlockedDoor;
				FreeBluePrint(ent);
				(*ent->free)(ent);
				playerData->inventory->keys--;
			}
		}
	}
	else
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

/**
*@brief Free functon for objects
*@param the entity calling it
*/
void BreakObject(Entity *ent)
{
	FreeEntity(ent);
	ent= NULL;
}
