#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <cmath>
#include "entity.h"
#include "collision.h"
#include "obj.h"

extern Map* map;
extern Entity* playerEnt;
void CreateObject(int x, int y,int width, int height, int frame)
{
	Entity *object;
	object= CreateEntity();
	object->sprite = LoadSprite("images/Resources1.png",width,height);
	object->sprite->animation[0]->startFrame = frame;
	object->sprite->animation[0]->currentFrame = frame;
	object->sprite->animation[0]->maxFrames = 0;
	object->currentAnimation = 0;
	object->position.x = x;
	object->position.y = y;
	object->speed = 15;
	object->dimensions.x = width;
	object->dimensions.y = height;
	object->hitBox.w = width;
	object->hitBox.h = height;
	object->hitBox.x = x;
	object->hitBox.y = y;
	object->whatAmI = 4;
	for(int i =0;i < map->numberOfRooms;++i)
	{
		if(AABB(object->hitBox,map->rooms[i].boundary))
		{
			object->room = &map->rooms[i];
		}
	}
	object->think = &ThinkObject;
	object->touch = &TouchObject;
	object->draw = &DrawObject;
	object->update = &UpdateObject;
	object->nextThink = 0;
}

void DrawObject(Entity* ent)
{
	DrawEntity(ent,ent->currentAnimation,ent->position.x,ent->position.y);
}
void ThinkObject(Entity* ent)
{
	ent->room = 
		   ent->position.x > ent->room->boundary.x + ent->room->boundary.w ? ent->room = ent->room->west 
		:  ent->position.x < ent->room->boundary.x ? ent->room = ent->room->east
		:  ent->position.y > ent->room->boundary.y + ent->room->boundary.h ? ent->room = ent->room->south
		:  ent->position.y < ent->room->boundary.y ? ent->room = ent->room->north
		:  ent->room;
	if(ent->nextThink > SDL_GetTicks())
	{
		ent->hitBox.w = ent->dimensions.x;
		ent->hitBox.h = ent->dimensions.y;
		ent->hitBox.y = ent->position.y;
		ent->hitBox.x = ent->position.x;
		if(OverlapsMap(map,ent).x != 0 || OverlapsMap(map,ent).y != 0)
		{
			Vec2DAdd(ent->position,ent->position,OverlapsMap(map,ent));
			ent->velocity.x = ent->velocity.y = 0;
		}
	}else if(ent->nextThink < SDL_GetTicks() && ent->temp == 1)
	{
		ent->velocity.x = ent->velocity.y = 0;
		ent->hitBox.y = ent->position.y;
		ent->hitBox.x = ent->position.x;
		ent->temp = 0;
		BreakObject(ent);
	}


}
void TouchObject(Entity* ent,Entity* other)
{
	if(ent->velocity.x == 0 && ent->velocity.y == 0)
		Vec2DAdd(other->position,other->position,CollisionObject(ent,other));
	else if(other->whatAmI == 2)
	{
		//Do some damage;
		BreakObject(ent);
	}
}
void UpdateObject(Entity* ent)
{
	playerEnt->objectHolding != ent ? ent->position = ent->position:
			ent->position = CreateVec2D(playerEnt->position.x-20,playerEnt->position.y- 50);
	Vec2DAdd(ent->position,ent->position,ent->velocity);
	EntityIntersectAll(ent);
}
void BreakObject(Entity *ent)
{
	FreeEntity(ent);
}