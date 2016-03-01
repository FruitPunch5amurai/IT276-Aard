#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include "vectors.h"
#include "entity.h"
#include "spirit.h"

int MaxSpirits = 6;
int SpiritIndex = 0;
extern Entity* playerEnt;
extern Entity* Spirits;

Entity* CreateSpirit(int x, int y)
{
	int i;
	Entity* spirit;

		spirit = CreateEntity();
		spirit->sprite = LoadSprite("images/AardSoul.png",21,32);
		spirit->position.x =x;
		spirit->position.y =y;
		spirit->currentAnimation = 0;
		spirit->sprite->animation[0]->oscillate = true;
		spirit->sprite->animation[0]->startFrame = 0;
		spirit->sprite->animation[0]->maxFrames = 5;
		spirit->nextThink = SDL_GetTicks() + 10;
		spirit->spiritState = BeingGuided;
		spirit->savedPlayerPosition = playerEnt->position;
		for(i =0;i < MaxSpirits;i++)
		{
			if(Spirits[i].inuse)
			{
				continue;
			}
			Spirits[i].inuse = 1;
			spirit->spiritIndex = i;
			break;
		}
		spirit->offset.x = 1;
		spirit->offset.y = 1;
		spirit->offset.x *= spirit->offset.x ;
		spirit->offset.y *= spirit->offset.y ;
		Spirits[spirit->spiritIndex];
		spirit->spiritIndex = SpiritIndex;
		SpiritIndex += 1;
		
		spirit->inuse = 1;
		spirit->speed = 9;
		spirit->think = &ThinkSpirit;
		spirit->touch = &TouchSpirit;
		spirit->draw = &DrawSpirit;
		spirit->update = &UpdateSpirit;
		return spirit;
}

void DrawSpirit(Entity* ent)
{
	DrawEntity(ent,ent->currentAnimation,ent->position.x,ent->position.y);
}

void UpdateSpirit(Entity* ent)
{
	if(ent->savedPlayerPosition.x  < ent->position.x)
	{
		ent->velocity.x = -ent->speed;
	}
	if(ent->savedPlayerPosition.x > ent->position.x)
	{	
		ent->velocity.x = +ent->speed;
	}
	if(ent->savedPlayerPosition.y < ent->position.y)
	{
		ent->velocity.y = -ent->speed;
	}
	if(ent->savedPlayerPosition.y > ent->position.y)
	{
		ent->velocity.y = +ent->speed;
	}
	if(ent->savedPlayerPosition.y == ent->position.y + ent->offset.y)
		ent->velocity.y = 0;
	if(ent->savedPlayerPosition.x == ent->position.x + ent->offset.x)
		ent->velocity.x = 0;
	if(abs(ent->position.x - ent->savedPlayerPosition.x) < ent->speed)
		ent->velocity.x = ent->savedPlayerPosition.x - ent->position.x;
	if(abs(ent->position.y - ent->savedPlayerPosition.y) < ent->speed)
		ent->velocity.y = ent->savedPlayerPosition.y - ent->position.y;
	Vec2DAdd(ent->position,ent->position ,ent->velocity);
	DrawSpirit(ent);
}
void ThinkSpirit(Entity *ent)
{
	if(ent->spiritState == BeingGuided && ent->nextThink < SDL_GetTicks() )
	{
		if(ent->savedPlayerPosition.x  < ent->position.x)
		{
			ent->offset.x *=10;
		}
		if(ent->savedPlayerPosition.x > ent->position.x)
		{	
			ent->offset.x *= -10;
		}
		if(ent->savedPlayerPosition.y < ent->position.y)
		{
			ent->offset.y *= -10;
		}
		if(ent->savedPlayerPosition.y > ent->position.y)
		{
			ent->offset.y *= 10;
		}
		Vec2DAdd(ent->savedPlayerPosition,playerEnt->position,ent->offset);
		printf("Spirit had a thought");
		ent->nextThink = SDL_GetTicks() + 1000;
	}

	
}
void TouchSpirit(Entity *ent,Entity *other)
{

}
void FreeSpirit(Spirit* spirit)
{

}