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
	enemy->sprite= LoadSprite("images/Enemies1.png",27,32);
	SetEnemyAnimations(enemy,"EnemyAnimationData.txt");
	enemy->currentAnimation = 1;

	enemy->savedPlayerPos = playerEnt->position;
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
	if(enemy->enemyType == 0)
	{
		enemy->update = &UpdateEnemyChaser;	
		enemy->think = &ThinkEnemyChaser;
	}
	else if(enemy->enemyType == 1){
		enemy->update = &UpdateEnemyLurker;
		enemy->think = &ThinkEnemyLurker;
	}

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
			moveIndicator = rand() % 3;
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
	Vec2D *dir = &CreateVec2D(-5.0,5.0);
	ent->hitBox.x =ent->position.x;
	ent->hitBox.y =ent->position.y;
	if(OverlapsMap(map,ent).x != 0)
	{

	}

	Vec2DAdd(ent->position,ent->position,ent->velocity);
	
}
void ThinkEnemyLurker(Entity *ent)
{
	if(ent->nextThink < SDL_GetTicks()){

		ent->nextThink = SDL_GetTicks() + 500;
	}	
}

void TouchEnemy(Entity *ent, Entity* other)
{
	if(other->isBeingGuided)
	{
		FreeSpirit(other);
		playerData->confidence -= 10;
		FreeEnemy(ent);
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
	int i;
	FILE *file;
	file = fopen(filename,"r");
	if(!file)
	{
		printf("Unable to read EnemyAnimationData");
		return;
	}
	i = 0;
	while(fscanf(file,"%s",buf) != EOF){
		if(strcmp(buf,"#") == 0)
			fgets(buf,50,file);
			fscanf(file,"%d",&j);
			ent->sprite->animation[i]->currentFrame = j;
			fscanf(file,"%d",&j);
			ent->sprite->animation[i]->startFrame = j;
			fscanf(file,"%d",&j);
			ent->sprite->animation[i]->maxFrames = j;
			fscanf(file,"%d",&j);
			ent->sprite->animation[i]->oscillate = j;
			i++;
		}
}

