#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <time.h>
#include <math.h>
#include <string>
#include "player.h"
#include "vectors.h"
#include "entity.h"
#include "collision.h"
#include "enemy.h"
//27x32
extern Map *map;
extern Entity *playerEnt;
extern Player *playerData;
int moveIndicator = 0;

Entity *CreateEnemy(int x, int y, int type)
{
	Entity *enemy = CreateEntity();
	srand(time(NULL));

	enemy->enemyType = type;
	if(type == 0)
	{
		enemy->sprite= LoadSprite("images/Enemies1.png",27,32);
		SetEnemyAnimations(enemy,"EnemyAnimationData.txt");
		enemy->update = &UpdateEnemyChaser;	
		enemy->think = &ThinkEnemyChaser;
		enemy->currentAnimation = 1;
	}
	else if(type == 1){
		enemy->sprite= LoadSprite("images/Enemies2.png",27,32);
		SetEnemyAnimations(enemy,"EnemyAnimationData.txt");
		enemy->update = &UpdateEnemyLurker;
		enemy->think = &ThinkEnemyLurker;
		enemy->currentAnimation = 1;
	}
	else if(type == 2)
	{
		enemy->sprite= LoadSprite("images/Enemies3.png",32,32);
		SetEnemyAnimations(enemy,"EnemyAnimationData.txt");
		enemy->update = &UpdateEnemySnatcher;	
		enemy->think = &ThinkEnemySnatcher;
		enemy->state = IDLE;
		enemy->currentAnimation = 0;
		enemy->sprite->animation[0]->frameRate = 500;

	}

	enemy->hitBox.x = enemy->position.x;
	enemy->hitBox.y = enemy->position.y;
	enemy->hitBox.w = enemy->sprite->w;
	enemy->hitBox.h= enemy->sprite->h;
	enemy->dimensions.x = enemy->sprite->w;
	enemy->dimensions.y = enemy->sprite->h;
	enemy->position.x = x;
	enemy->position.y = y;
	enemy->speed = 2;
	enemy->velocity.x = -enemy->speed;

	enemy->whatAmI = 2;
	enemy->state = IDLE;
	enemy->nextThink = SDL_GetTicks() + 500;

	enemy->draw = &DrawEnemy;
	enemy->touch = &TouchEnemy;

	return enemy;
}
void DrawEnemy(Entity *ent)
{
	DrawEntity(ent,ent->currentAnimation,ent->position.x,ent->position.y);
}
void UpdateEnemyChaser(Entity *ent)
{
	ent->hitBox.x =ent->position.x;
	ent->hitBox.y =ent->position.y;
	if(ent->state == MOVING){
		if(moveIndicator == 0)
			ent->velocity.x = ent->speed;
		if(moveIndicator == 1)
			ent->velocity.y = ent->speed;
		if(moveIndicator == 2)
			ent->velocity.x = -ent->speed;
		if(moveIndicator == 3)
			ent->velocity.y = -ent->speed;
		}
	else if (ent->state == CHASING)
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
	
			}
		else
		{
			ent->velocity.x = ent->velocity.y = 0;
		}

	if(OverlapsMap(map,ent).x != 0)
	{
		Vec2DAdd(ent->position,ent->position,OverlapsMap(map,ent));
		ent->state = IDLE;
	}else{
		Vec2DAdd(ent->position,ent->position,ent->velocity);
	}
	EntityIntersectAll(ent);
}
void ThinkEnemyChaser(Entity *ent)
{
	
	if(ent->nextThink < SDL_GetTicks()){
		ent->savedPlayerPos = playerEnt->position;
		ent->nextMove += 1;
		printf("%d\n",ent->nextMove);
		if(DistanceBetweenLessThan2D(ent->position,ent->savedPlayerPos,150))
		{
			ent->nextMove = 0;
			ent->state = CHASING;
		}		
		else if(ent->nextMove >= 5 && ent->state != MOVING)
		{
			ent->nextMove = 0;
			moveIndicator = rand() % 4;
			ent->state = MOVING;
		}
		else if(ent->nextMove >= 2 && ent->state != IDLE ) 
		{
			ent->nextMove = 0;
			ent->state = IDLE;
		}
		ent->nextThink = SDL_GetTicks() + 500;
	}	
}
void UpdateEnemyLurker(Entity *ent)
{
	ent->hitBox.x =ent->position.x;
	ent->hitBox.y =ent->position.y;
		if(ent->state == MOVING){
		if(moveIndicator == 0)
			ent->velocity.x = ent->speed;
		if(moveIndicator == 1)
			ent->velocity.y = ent->speed;
		if(moveIndicator == 2)
			ent->velocity.x = -ent->speed;
		if(moveIndicator == 3)
			ent->velocity.y = -ent->speed;
		}
	if(OverlapsMap(map,ent).x != 0)
	{
		Vec2DAdd(ent->position,ent->position,OverlapsMap(map,ent));
		moveIndicator = rand() % 4;
	}
	if(ent->sprite->animation[1]->currentFrame == ent->sprite->animation[1]->maxFrames)
		ent->sprite->animation[1]->frameRate = 5000;
	else
		ent->sprite->animation[1]->frameRate = 100;
	Vec2DAdd(ent->position,ent->position,ent->velocity);
	EntityIntersectAll(ent);
}
void ThinkEnemyLurker(Entity *ent)
{
	if(ent->nextThink < SDL_GetTicks()){
		ent->state = MOVING;
		moveIndicator = rand() % 4;
		ent->velocity.x = 0;
		ent->velocity.y = 0;
		ent->nextThink = SDL_GetTicks() + 5000;
	}	
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
 EntityIntersectAll(ent);
}
void ThinkEnemySnatcher(Entity *ent)
{
	if(ent->nextThink < SDL_GetTicks()){
		if(ent->state == IDLE || ent->state == GOINGUP)
			ent->savedPlayerPos = playerEnt->position ;
		if(DistanceBetweenLessThan2D(ent->position,playerEnt->position,150) 
		&& (ent->state != GOINGUP && ent->state != GOINGDOWN))
	{
		ent->state = GOINGUP;
		ent->temp = SDL_GetTicks() + 1000;
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
	if(other->isBeingGuided)
	{
		FreeSpirit(other);
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
}
void SetEnemyAnimations(Entity *ent,char *filename)
{
	char buf[50];
	int j;
	int i,numAnimations;
	FILE *file;
	file = fopen(filename,"r");
	if(!file)
	{
		printf("Unable to read EnemyAnimationData");
		return;
	}
	i = 0;
	if(ent->enemyType == 0){
	while(fscanf(file,"%s",buf) != EOF){
		if(strcmp(buf,"#Chaser") == 0)
		{
			fscanf(file,"%s",buf);
			fscanf(file,"%d",&numAnimations);
			for(i = 0;i < numAnimations;i++){ 
				fscanf(file,"%s",buf);
				fscanf(file,"%d",&j);
				ent->sprite->animation[i]->currentFrame = j;
				fscanf(file,"%d",&j);
				ent->sprite->animation[i]->startFrame = j;
				fscanf(file,"%d",&j);
				ent->sprite->animation[i]->maxFrames = j;
				fscanf(file,"%d",&j);
				ent->sprite->animation[i]->oscillate = j;
			}
		}
	}
	}
	if(ent->enemyType == 1){
	while(fscanf(file,"%s",buf) != EOF){
		if(strcmp(buf,"#Lurker") == 0)
		{
			fscanf(file,"%s",buf);
			fscanf(file,"%d",&numAnimations);
			for(i = 0;i < numAnimations;i++){ 
				fscanf(file,"%s",buf);
				fscanf(file,"%d",&j);
				ent->sprite->animation[i]->currentFrame = j;
				fscanf(file,"%d",&j);
				ent->sprite->animation[i]->startFrame = j;
				fscanf(file,"%d",&j);
				ent->sprite->animation[i]->maxFrames = j;
				fscanf(file,"%d",&j);
				ent->sprite->animation[i]->oscillate = j;
			}
		}
	}
	}
	if(ent->enemyType == 2){
	while(fscanf(file,"%s",buf) != EOF){
		if(strcmp(buf,"#Snatcher") == 0)
		{
			fscanf(file,"%s",buf);
			fscanf(file,"%d",&numAnimations);
			for(i = 0;i < numAnimations;i++){ 
				fscanf(file,"%s",buf);
				fscanf(file,"%d",&j);
				ent->sprite->animation[i]->currentFrame = j;
				fscanf(file,"%d",&j);
				ent->sprite->animation[i]->startFrame = j;
				fscanf(file,"%d",&j);
				ent->sprite->animation[i]->maxFrames = j;
				fscanf(file,"%d",&j);
				ent->sprite->animation[i]->oscillate = j;
			}
		}
	}
	}
}

