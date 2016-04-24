#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <time.h>
#include <math.h>
#include <string>
#include <jansson.h>
#include "player.h"
#include "vectors.h"
#include "entity.h"
#include "collision.h"
#include "enemy.h"
//27x32
extern Map *map;
extern Entity *playerEnt;
extern Player *playerData;

Entity *CreateEnemy(int x, int y, EnemyType type)
{
	Entity *enemy = CreateEntity();
	srand(time(NULL));

	enemy->enemyType = type;
	json_t *root;
	json_error_t err;
	root = json_load_file("AnimationData.json",0,&err);
	if(!root)
	{
		fprintf(stderr,"error: on line %d : %s\n", err.line,err.text);
		exit(1);
	}

		json_t *data , *Entity, *Animations;
		char *ENT ,*filepath; 
		int frameW, frameH,id;
		data = json_object_get(root,"AnimationData");
		for(int i = 0;i < json_array_size(data);++i)
		{
			Entity = json_array_get(data,i);
			json_unpack(Entity,"{s:s,s:i,s:s,s:i,s:i}","Entity",&ENT,"EnemyType",&id,"filepath",&filepath,"frame-width",&frameW,"frame-height",&frameH);
			if(id == type){
			enemy->sprite = LoadSprite(filepath,frameW,frameH);
			data = json_object_get(Entity,"Animations");
			if(!json_is_array(data))
			{
				fprintf(stderr, "error: commit %d: Not an array\n", 0);
				json_decref(root);
				exit(1);
			}
		for(int i = 0;i < json_array_size(data);++i)
		{
			Animations = json_array_get(data,i);	
			if(!json_is_object(Animations))
			{
				fprintf(stderr, "error: commit %d: Not an object\n", 0);
				json_decref(root);
				exit(1);
			}

			enemy->sprite->animation[i].startFrame = json_number_value(json_object_get(Animations,"start-frame"));
			enemy->sprite->animation[i].currentFrame = enemy->sprite->animation[i].startFrame;
			enemy->sprite->animation[i].maxFrames = json_number_value(json_object_get(Animations,"max-frames"));
			enemy->sprite->animation[i].oscillate = json_number_value(json_object_get(Animations,"oscillate"));
			enemy->sprite->animation[i].holdFrame = 0;
			enemy->sprite->animation[i].frameInc = 1;
			if(json_object_get(Animations,"held-frame"))
				enemy->sprite->animation[i].heldFrame = json_number_value(json_object_get(Animations,"held-frame"));
			else
				enemy->sprite->animation[i].heldFrame = -1;
		}
		}else{
			continue;
		}
	}
	json_decref(root);
	if(type == Chaser)
	{
		enemy->sprite->fpl = 1;
		enemy->speed = 2;
		enemy->update = &UpdateEnemyChaser;	
		enemy->think = &ThinkEnemyChaser;
		enemy->currentAnimation = 0;
		enemy->EnemyHP = 10;
	}
	else if(type == Lurker){
		enemy->sprite->fpl = 1;
		enemy->speed = 4;
		enemy->update = &UpdateEnemyLurker;
		enemy->think = &ThinkEnemyLurker;
		enemy->currentAnimation = 0;
		enemy->EnemyHP = 10;
	}
	else if(type == Snatcher)
	{
		enemy->update = &UpdateEnemySnatcher;	
		enemy->think = &ThinkEnemySnatcher;
		enemy->state = IDLE;
		enemy->speed = 2;
		enemy->sprite->animation[0].frameRate = 500;
		enemy->EnemyHP = 10;
	}
	enemy->dimensions.x = enemy->sprite->w;
	enemy->dimensions.y = enemy->sprite->h;
	enemy->position.x = x;
	enemy->position.y = y;
	enemy->position2.x = x;
	enemy->position2.y = y;
	enemy->hitBox.x = enemy->position.x;
	enemy->hitBox.y = enemy->position.y;
	enemy->hitBox.w = enemy->sprite->w;
	enemy->hitBox.h= enemy->sprite->h;
	enemy->velocity.x = -enemy->speed;

		for(int i =0;i < map->numberOfRooms;++i)
		{	
			if(AABB(enemy->hitBox,map->rooms[i].boundary))
			{
				enemy->room = &map->rooms[i];
			}
		}

	enemy->whatAmI = Enemy;
	enemy->state = IDLE;
	enemy->nextThink = SDL_GetTicks() + 500;
	enemy->moveIndicator = 0;
	enemy->draw = &DrawEnemy;
	enemy->touch = &TouchEnemy;
	enemy->free = &FreeEnemy;

	return enemy;
}
void DrawEnemy(Entity *ent)
{
	DrawEntity(ent,ent->currentAnimation,ent->position.x,ent->position.y);
}
void UpdateEnemyChaser(Entity *ent)
{
	Vec2D n;
	ent->hitBox.x =ent->position.x;
	ent->hitBox.y =ent->position.y;
	if(ent->state == MOVING && !ent->knockback){
		if(ent->moveIndicator  == 0)
			ent->velocity.x = ent->speed;
		if(ent->moveIndicator == 1)
			ent->velocity.y = ent->speed;
		if(ent->moveIndicator == 2)
			ent->velocity.x = -ent->speed;
		if(ent->moveIndicator == 3)
			ent->velocity.y = -ent->speed;
		}
	else if (ent->state == CHASING &&!ent->knockback)
		{
			Vec2DSub(n,ent->savedPlayerPos,ent->position,);
			Vec2DNormalize(&n);
			n.x = n.x < 0 ? floor(n.x): ceil(n.x);
			n.y = n.y < 0 ? floor(n.y): ceil(n.y);
			ent->velocity.x = ent->speed * n.x ;
			ent->velocity.y = ent->speed * n.y ;
			}
	else if(ent->state == IDLE && !ent->knockback)
		{
			ent->velocity.x = ent->velocity.y = 0;
		}

	if(OverlapsMap(map,ent).x != 0)
	{
		Vec2DAdd(ent->position,ent->position,OverlapsMap(map,ent));
		ent->state = IDLE;
	}
	else if(playerEnt->room->id != ent->room->id)
	{
		ent->position = ent->position2;
	}
	else if(ent->position.y >= ent->room->boundary.y +ent->room->boundary.h ||
		ent->position.x >= ent->room->boundary.x +ent->room->boundary.w ||
		ent->position.x <= ent->room->boundary.x || ent->position.y <= ent->room->boundary.y)
	{
		Vec2DAdd(ent->position,ent->position,-ent->velocity);
		ent->state = IDLE;
	}
	else{
		Vec2DAdd(ent->position,ent->position,ent->velocity);
	}
	EntityIntersectAll(ent);
}
void ThinkEnemyChaser(Entity *ent)
{
	if(ent->EnemyHP <= 0)
		FreeEnemy(ent);
	if(ent->nextThink < SDL_GetTicks()){
		ent->savedPlayerPos = playerEnt->position;
		ent->nextMove += 1;
		if(ent->knockback == 2)
		{
			ent->knockback = 0;
		}
		if(ent->knockback == 1)
		{
			ent->nextThink =  SDL_GetTicks() + 500;
			ent->knockback = 2;
		}
		else if(DistanceBetweenLessThan2D(ent->position,ent->savedPlayerPos,150))
		{
			ent->nextMove = 0;
			ent->state = CHASING;
		}		
		else if(ent->nextMove >= 5 && ent->state != MOVING)
		{
			ent->nextMove = 0;
			ent->moveIndicator = rand() % 4;
			ent->state = MOVING;
			ent->currentAnimation = 1;
		}
		else if(ent->nextMove >= 2 && ent->state != IDLE ) 
		{
			ent->nextMove = 0;
			ent->state = IDLE;
			ent->currentAnimation = 0;
		}
		ent->nextThink = SDL_GetTicks() + 500;
	}	
}
void UpdateEnemyLurker(Entity *ent)
{
	ent->hitBox.x =ent->position.x;
	ent->hitBox.y =ent->position.y;
		if(ent->state == MOVING && !ent->knockback){
		if(ent->moveIndicator == 0)
			ent->velocity.x = ent->speed;
		if(ent->moveIndicator == 1)
			ent->velocity.y = ent->speed;
		if(ent->moveIndicator == 2)
			ent->velocity.x = -ent->speed;
		if(ent->moveIndicator == 3)
			ent->velocity.y = -ent->speed;
		}
	if(ent->position.x + ent->dimensions.x >= ent->room->boundary.x +ent->room->boundary.w ||
		ent->position.x <= ent->room->boundary.x)
	{
		Vec2DAdd(ent->position,ent->position,-ent->velocity);
		ent->moveIndicator = rand() % 4;
	}
	else if(ent->position.y >= ent->room->boundary.y +ent->room->boundary.h ||
		ent->position.y <= ent->room->boundary.y)
	{
		Vec2DAdd(ent->position,ent->position,-ent->velocity);
		ent->moveIndicator = rand() % 4;
	}
	else if(playerEnt->room->id != ent->room->id)
	{
		ent->position = ent->position2;
	}
	else if(OverlapsMap(map,ent).x != 0)
	{
		Vec2DAdd(ent->position,ent->position,OverlapsMap(map,ent));
		ent->velocity = OverlapsMap(map,ent);
	}
	else{
		Vec2DAdd(ent->position,ent->position,ent->velocity);
	}
	/*
	if(ent->sprite->animation[1]->currentFrame == ent->sprite->animation[1]->maxFrames)
		ent->sprite->animation[1]->frameRate = 5000;
	else
		ent->sprite->animation[1]->frameRate = 100;
		*/
	EntityIntersectAll(ent);
}
void ThinkEnemyLurker(Entity *ent)
{
	if(ent->EnemyHP <= 0)
		FreeEnemy(ent);
	if(ent->nextThink < SDL_GetTicks()){
		if(ent->knockback == 2)
		{
			ent->knockback = 0;
		}
		if(ent->knockback == 1)
		{
			ent->nextThink =  SDL_GetTicks() + 500;
			ent->knockback = 2;
		}else
		{
		ent->state = MOVING;
		ent->moveIndicator = rand() % 4;
		ent->velocity.x = 0;
		ent->velocity.y = 0;
		ent->nextThink = SDL_GetTicks() + 4000;
		}
	}
	//if(OverlapsMap(map,ent).x != 0 || OverlapsMap(map,ent).y != 0)
		ent->moveIndicator = 
			OverlapsMap(map,ent).x < 0 ? 2:
			OverlapsMap(map,ent).x > 0 ? 0:
			OverlapsMap(map,ent).y < 0 ? 3:
			OverlapsMap(map,ent).y > 0 ? 1:
			ent->moveIndicator;
}
void UpdateEnemySnatcher(Entity *ent)
{

 if (ent->state == GOINGUP && ent->temp > SDL_GetTicks())
	{
	ent->hitBox.w= 0;
	ent->hitBox.h= 0;
	ent->hitBox.x =0;
	ent->hitBox.y =0;
	Vec2DAdd(ent->position,ent->position,CreateVec2D(0,-4));
    }
 else if (ent->state == GOINGUP && ent->temp < SDL_GetTicks())
	{
	 ent->position.x = ent->savedPlayerPos.x;
	 ent->position.y = ent->savedPlayerPos.y - 200;
	 ent->state = GOINGDOWN;
	}
 if(ent->state == GOINGDOWN)
	{
	Vec2DAdd(ent->position,ent->position,CreateVec2D(0,4));
	}
 if(ent->state == IDLE){
	ent->hitBox.w = ent->sprite->w;
	ent->hitBox.h= ent->sprite->h;
	ent->hitBox.x =ent->position.x;
	ent->hitBox.y =ent->position.y;
 }
 	if(playerEnt->room->id != ent->room->id)
	{
		ent->position = ent->position2;
	}
 EntityIntersectAll(ent);
}
void ThinkEnemySnatcher(Entity *ent)
{
	if(ent->EnemyHP <= 0)
		FreeEnemy(ent);
	if(ent->nextThink < SDL_GetTicks()){
		if(ent->state == IDLE || ent->state == GOINGUP)
			ent->savedPlayerPos = playerEnt->position ;
		if(DistanceBetweenLessThan2D(ent->position,playerEnt->position,150) 
		&& (ent->state != GOINGUP && ent->state != GOINGDOWN))
	{
		ent->state = GOINGUP;
		ent->temp = SDL_GetTicks() + 500;
	}
		if(ent->position.y >= ent->savedPlayerPos.y && ent->state == GOINGDOWN)
	{
		ent->state = IDLE;
	}
		ent->nextThink = SDL_GetTicks() + 100;
	}
}

void TouchEnemy(Entity *ent, Entity* other)
{
	if(other->spiritState == BeingGuided)
	{
		FreeSpirit(other);
		map->numOfSpirits--;
		playerData->guidingSpirits	-=	1;
		playerData->confidence -= 10;
		FreeEnemy(ent);
	}else if(other->whatAmI == 0)
	{
		ent->state = IDLE;
	}
}
void FreeEnemy(Entity *ent)
{
	FreeEntity(ent);
	ent= NULL;
}


