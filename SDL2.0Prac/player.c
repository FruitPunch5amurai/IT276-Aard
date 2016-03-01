#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include "sprite.h"
#include "vectors.h"
#include "player.h"

Player *playerData = NULL;
Entity* playerEnt= NULL;

Entity* Spirits;

extern Map *map;
/**
*@brief Loads in Data for AnimationData for the player from a .txt
*/
void LoadPlayerAnimations(Entity* ent)
{
	int j,i=0;
	FILE *file;
	file = fopen("AnimationData.txt","r");

	if(!file)
	{
		printf("Unable to read AnimationData.txt");
		return;
	}
	while(!feof(file))
	{
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
/**
*@brief Creates and Allocates memory for the player. Then initializes all the
*@brief data members for Player
*/
void CreatePlayer()
{
	playerEnt = CreateEntity();

	playerData = (Player*)malloc(sizeof(Player));
	memset(playerData,0,sizeof(Player));

	Spirits = (Entity*)malloc(sizeof(Entity)*6);
	memset(Spirits,0,sizeof(Entity));

	//Data for Idle Animation
	playerEnt->sprite= LoadSprite("images/Sonic.png",32,42);
	LoadPlayerAnimations(playerEnt);
	playerEnt->dimensions.x = playerEnt->sprite->w;
	playerEnt->dimensions.y = playerEnt->sprite->h;
	playerEnt->position.x = 300;
	playerEnt->position.y = 300;
	playerEnt->entType = 0;
	playerEnt->position2.x = playerEnt->position.x+playerEnt->dimensions.x-1;
	playerEnt->position2.y = playerEnt->position.y+playerEnt->dimensions.y-1;
	playerEnt->speed = 10;
	
	/*
	player->self->sprite->animation[0]->startFrame = 5;
	player->self->sprite->animation[0]->maxFrames = 5;
	player->self->sprite->animation[0]->oscillate = true;

	//Data for Run Animation
	player->self->sprite->animation[1]->maxFrames = 6;
	player->self->sprite->animation[1]->oscillate = true;
	player->self->sprite->animation[1]->startFrame = 17;
	player->self->sprite->animation[1]->currentFrame = 17;
	*/

	/*Set the PlayerData*/
	playerData->confidence = 30;
	playerData->rescuedSpirits = 0;
	playerData->guidingSpirits = 0;
	playerData->abandonSpirits = 0;
	

	playerEnt->update = &UpdatePlayer;
	playerEnt->draw = &DrawPlayer;
	playerEnt->think = &ThinkPlayer;
	playerEnt->touch = &TouchPlayer;

}
/**
*@brief Draws specified Entity to main renderer
*/
void DrawPlayer(Entity* ent)
{
	DrawEntity(ent,ent->currentAnimation,ent->position.x,ent->position.y);
}
/**
*@brief Updates specified Entity
*/
void UpdatePlayer(Entity *ent)
{
	if(ent->velocity.x != 0 || ent->velocity.y != 0)
		ent->currentAnimation= 1;
	else
		ent->currentAnimation = 0;
	Vec2DAdd(ent->position,ent->position,ent->velocity);
	DrawPlayer(ent);
}
void ThinkPlayer(Entity *ent)
{
	if(ent->velocity.x < 0)
	{
		if(OverlapsMap(map,ent)){
			printf("Stepped on Solid!");
		}
	}
	if(ent->velocity.x > 0)
	{
		if(OverlapsMap(map,ent)){
			printf("Stepped on Solid!");
		}
	}
	if(ent->velocity.y < 0)
	{
		if(OverlapsMap(map,ent)){
			printf("Stepped on Solid!");
		}
	}
	if(ent->velocity.y > 0)
	{
		if(OverlapsMap(map,ent)){
			printf("Stepped on Solid!");
		}
	}
}
void TouchPlayer(Entity *ent,Entity *other)
{

}
/**
*@brief Deallocates memory for player
*/
void FreePlayer()
{
	FreeEntity(playerEnt);
	free(playerEnt);
	playerEnt = NULL;
}

/**
*@brief Returns player if not null
*/
Entity *GetPlayer()
{
	if(!playerEnt)
	{
		return playerEnt;
	}
	printf("Player does not exist.");
}