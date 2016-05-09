#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <glib.h>
#include <string>
#include "graphics.h"
#include "vectors.h"
#include "collision.h"
#include "enemy.h"
#include "player.h"
#include "spirit.h"

int MaxSpirits = 6;
extern Map *map;
extern Entity* playerEnt;
extern Player* playerData;
GList *SpiritTrain = NULL;
int MAXSPIRITPARTICLES = 2;
Entity* CreateSpirit(int x, int y)
{
	int i;
	Entity* spirit;
		spirit = CreateEntity();
		spirit->particles = (Particle*)malloc(sizeof(Particle) * MAXSPIRITPARTICLES );
		memset(spirit->particles,0,sizeof(Particle) * MAXSPIRITPARTICLES );	
	
		spirit->sprite = LoadSprite("images/AardSoul.png",21,32);
		spirit->whatAmI = Spirit;
		spirit->position.x =x;
		spirit->position.y =y;
		spirit->dimensions.x = spirit->sprite->w;
		spirit->dimensions.y = spirit->sprite->h;		
		spirit->hitBox.x =x;
		spirit->hitBox.y =y;
		spirit->hitBox.w = spirit->sprite->w;
		spirit->hitBox.h = spirit->sprite->h;

		spirit->spiritIndex = -1;
		spirit->value = 2;
 		spirit->currentAnimation = 0;
		spirit->sprite->animation[0].oscillate = true;
		spirit->sprite->animation[0].startFrame = 0;
		spirit->sprite->animation[0].maxFrames = 5;
		spirit->nextThink = SDL_GetTicks() + 10;
  		spirit->spiritState = Untouched;
 		for(int i =0;i < map->numberOfRooms;++i)
		{	
			if(AABB(spirit->hitBox,map->rooms[i].boundary))
			{
				spirit->room = &map->rooms[i];
			}
		}
		spirit->offset.x = 20;
		spirit->offset.y = 20;
		
	 for(i = 0; i < MAXSPIRITPARTICLES ; ++i )
		{
			spirit->particles[i] = *CreateParticle( spirit->position.x,spirit->position.y,
				"images/Particles.png",9,12,20);
		}
		g_hash_table_insert(spirit->sounds,"PickUp",Mix_LoadWAV("sound/spirit_PickUp.wav"));
		spirit->follow = NULL;
		spirit->inuse = 1;
		spirit->speed = 4;
		spirit->think = &ThinkSpirit;
		spirit->touch = &TouchSpirit;
		spirit->draw = &DrawSpirit;
		spirit->update = &UpdateSpirit;
		spirit->free = &FreeSpirit;
		return spirit;
		
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
	if(ent->room->id != playerEnt->room->id)
		ent->position = playerEnt->position;
	}
	Vec2DAdd(ent->position,ent->position ,ent->velocity);
	Vec2DAdd(ent->position,ent->position,OverlapsMap(map,ent));
	EntityIntersectAll(ent);
}
void ThinkSpirit(Entity *ent)
{
	if(ent->spiritIndex >= 0){
		ent->follow =  (Entity*)g_list_nth_data(SpiritTrain,ent->spiritIndex-1);
	}
	if(ent->spiritState == BeingGuided && ent->nextThink < SDL_GetTicks() )
	{		
		if(ent->follow != NULL)
			ent->savedPlayerPos = ent->follow->position;
		ent->nextThink = SDL_GetTicks() + 200;
		if(!DistanceBetweenLessThan2D(ent->position,playerEnt->position,200))
		{
			playerEnt->penalty -= 1;
			ent->spiritState = Lost;
			ent->think = &ThinkEnemyLurker;
			ent->update = &UpdateEnemyLurker;
			ent->speed = 1;
			SpiritTrain = g_list_remove(SpiritTrain,ent);
		}
	}

	if(ent->timer != NULL){
		if(ent->timer->temp > 0){
			ent->hitBox.w = 0;
			ent->hitBox.h = 0;
			ent->hitBox.x = 0;
			ent->hitBox.y = 0;
		}else{
			ent->immunity = 0;
			ent->hitBox.w = ent->sprite->w;
			ent->hitBox.h = ent->sprite->h;	
			ent->hitBox.x = ent->position.x;
			ent->hitBox.y = ent->position.y;
		}
	}
		ent->room = 
		   ent->position.x > ent->room->boundary.x + ent->room->boundary.w ? ent->room = ent->room->west 
		:  ent->position.x < ent->room->boundary.x ? ent->room = ent->room->east
		:  ent->position.y > ent->room->boundary.y + ent->room->boundary.h ? ent->room = ent->room->south
		:  ent->position.y < ent->room->boundary.y ? ent->room = ent->room->north
		:  ent->room;

}
void TouchSpirit(Entity *ent,Entity *other)
{
		if(other->whatAmI == 0)//Its a player!
	{
		if(ent->spiritState == Untouched)
		{
			ent->spiritState= BeingGuided;	
			ent->spiritIndex = g_list_length(SpiritTrain);
			playerData->guidingSpirits+=1;
			ent->speed = 4;
			SpiritTrain = g_list_append(SpiritTrain,ent);
			Mix_PlayChannel(-1,(Mix_Chunk*)g_hash_table_lookup(ent->sounds,"PickUp"),0);
		}
		if(ent->spiritState == Lost){
			playerEnt->penalty += playerEnt->penalty < 0 ? 1 : 0;
			playerData->guidingSpirits	+=	1;
			ent->spiritState= BeingGuided;	
			ent->spiritIndex = g_list_length(SpiritTrain);
			ent->think = &ThinkSpirit;
			ent->update = &UpdateSpirit;
			ent->position = playerEnt->position;
			ent->speed = 4;
			SpiritTrain = g_list_append(SpiritTrain,ent);
		}
	}
}
void ClearSpiritLink()
{
	Entity* ent;
	for(int i = 1; i > g_list_length(SpiritTrain);++i)
	{
		ent = (Entity*)g_list_nth_data(SpiritTrain,i);
		SpiritTrain = g_list_remove(SpiritTrain,g_list_nth_data(SpiritTrain,i));
		FreeSpirit(ent);
	}
}
void FreeSpirit(Entity* ent)
{
	int i ;
	if(ent->spiritIndex > 0)
	{
	SpiritTrain = g_list_remove(SpiritTrain,ent);
	}
	ent->spiritIndex = NULL;
	FreeEntity(ent);
	ent= NULL;
}
