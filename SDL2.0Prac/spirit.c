#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include "graphics.h"
#include "vectors.h"
#include "entity.h"
#include "LList.h"
#include "spirit.h"

int MaxSpirits = 6;
extern Entity* playerEnt;
extern Entity* playerData;
Link *l  = CreateLink(NULL,NULL,NULL);
ELink *SpiritLink = CreateELink();
int MAXSPIRITPARTICLES = 2;
void CreateSpirit(int x, int y)
{
	int i;
	Entity* spirit;
		spirit = CreateEntity();
		spirit->particles = (Particle*)malloc(sizeof(Particle) * MAXSPIRITPARTICLES );
		memset(spirit->particles,0,sizeof(Particle) * MAXSPIRITPARTICLES );	
	
		spirit->sprite = LoadSprite("images/AardSoul.png",21,32);
		spirit->whatAmI = 1;
		spirit->isBeingGuided = 0;
		spirit->position.x =x;
		spirit->position.y =y;
		spirit->dimensions.x = spirit->sprite->w;
		spirit->dimensions.y = spirit->sprite->h;		
		spirit->hitBox.x =x;
		spirit->hitBox.y =y;
		spirit->hitBox.w = spirit->sprite->w;
		spirit->hitBox.h = spirit->sprite->h;

		spirit->spiritIndex = -1;
		spirit->currentAnimation = 0;
		spirit->sprite->animation[0]->oscillate = true;
		spirit->sprite->animation[0]->startFrame = 0;
		spirit->sprite->animation[0]->maxFrames = 5;
		spirit->nextThink = SDL_GetTicks() + 10;
		spirit->spiritState = 0;

		//moveToEnd(SpiritLink);
		//Insert(SpiritLink,spirit);
		spirit->offset.x = 20;
		spirit->offset.y = 20;
		
	 for(i = 0; i < MAXSPIRITPARTICLES ; ++i )
		{
			spirit->particles[i] = *CreateParticle( spirit->position.x,spirit->position.y,
				"images/Particles.png",9,12,20);
		}

		spirit->follow = NULL;
		spirit->inuse = 1;
		spirit->speed = 4;
		spirit->think = &ThinkSpirit;
		spirit->touch = &TouchSpirit;
		spirit->draw = &DrawSpirit;
		spirit->update = &UpdateSpirit;

		
}

void DrawSpirit(Entity* ent)
{
	int i;

	DrawEntity(ent,ent->currentAnimation,ent->position.x,ent->position.y);
	if(!ent->follow){
	for(i = 0; i < MAXSPIRITPARTICLES ; i++){
		if(isDead(&ent->particles[i]))
		{
			ent->particles[i] = *CreateParticle( ent->position.x,ent->position.y,
				"images/Particles.png",9,12,20);	
		}
	}
	for(i = 0; i < MAXSPIRITPARTICLES ; i++){
	DrawParticle(&ent->particles[i],0, ent->particles[i].position.x, ent->particles[i].position.y);
	}
	}
}

void UpdateSpirit(Entity* ent)
{
	ent->hitBox.x =ent->position.x;
	ent->hitBox.y =ent->position.y;
	if(ent->follow != NULL){
	if(abs(ent->position.x - ent->savedPlayerPos.x) < ent->offset.x &&
		abs(ent->position.y - ent->savedPlayerPos.y) < ent->offset.y){
	ent->velocity.x = 0;
	ent->velocity.y = 0;
	}else
	{
	if(ent->savedPlayerPos.x < ent->position.x)
	{
		ent->velocity.x = -ent->speed;
	}
	if(ent->savedPlayerPos.x > ent->position.x)
	{	
		ent->velocity.x = +ent->speed;
	}
	if(ent->savedPlayerPos.y< ent->position.y)
	{
		ent->velocity.y = -ent->speed;
	}
	if(ent->savedPlayerPos.y > ent->position.y)
	{
		ent->velocity.y = +ent->speed;
	}
	
	if(abs(ent->position.x - ent->savedPlayerPos.x) < ent->speed)
		ent->velocity.x = ent->savedPlayerPos.x - ent->position.x;
	if(abs(ent->position.y - ent->savedPlayerPos.y) < ent->speed)
		ent->velocity.y = ent->savedPlayerPos.y - ent->position.y;
		}
	}
	Vec2DAdd(ent->position,ent->position ,ent->velocity);
	DrawSpirit(ent);
}
void ThinkSpirit(Entity *ent)
{
	EntityIntersectAll(ent);
	if(ent->spiritIndex >= 0){
		moveToPos(SpiritLink,ent->spiritIndex);
		if(Prev(SpiritLink->curr)->curr)
			ent->follow = Prev(SpiritLink->curr)->curr;
	}
	if(ent->spiritState == BeingGuided && ent->nextThink < SDL_GetTicks() )
	{		
		if(ent->follow != NULL)
			ent->savedPlayerPos = ent->follow->position;
		ent->nextThink = SDL_GetTicks() + 200;
	}
	
}
void TouchSpirit(Entity *ent,Entity *other)
{
		if(other->whatAmI == 0)//Its a player!
	{
		if(!ent->isBeingGuided)
		{
			moveToEnd(SpiritLink);
			Insert(SpiritLink,ent);
			ent->isBeingGuided = 1;	
			ent->spiritIndex = SpiritLink->count;
		}
	}
}
void FreeSpirit(Entity* ent)
{
	int i ;
	moveToPos(SpiritLink,ent->spiritIndex);
	Remove(SpiritLink);
	if(Next(SpiritLink->curr)->curr != NULL)
	{
		for(i = SpiritLink->curr->curr->spiritIndex+1; i <= SpiritLink->count;i++) 
		{
			moveToPos(SpiritLink,i);
			if(SpiritLink->curr != NULL)
			{
				SpiritLink->curr->curr->spiritIndex--;
			}
		}
	}
	FreeEntity(ent);

}
