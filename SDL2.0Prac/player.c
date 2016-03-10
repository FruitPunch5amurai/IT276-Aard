#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include "sprite.h"
#include"graphics.h"
#include "vectors.h"
#include "LList.h"
#include "player.h"

SDL_Color hp;
SDL_Color hp2;
Player *playerData = NULL;
Entity* playerEnt= NULL;
extern Link *l;
extern ELink *SpiritLink;
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

	//Data for Idle Animation
	playerEnt->sprite= LoadSprite("images/Sonic.png",32,42);
	LoadPlayerAnimations(playerEnt);
	playerEnt->whatAmI = 0;
	playerEnt->dimensions.x = playerEnt->sprite->w;
	playerEnt->dimensions.y = playerEnt->sprite->h;
	
	hp.b = 0;hp.r = 255;hp.g = 0;hp.a = 0;
	hp2.b= 255; hp2.r = 0; hp2.g = 0;hp2.a = 255;
	
	playerEnt->position.x = 400;
	playerEnt->position.y = 400;
	playerEnt->hitBox.x = playerEnt->position.x;
	playerEnt->hitBox.y = playerEnt->position.y;
	playerEnt->hitBox.w = playerEnt->sprite->w;
	playerEnt->hitBox.h= playerEnt->sprite->h;
	playerEnt->entType = 0;
	playerEnt->position2.x = playerEnt->position.x+playerEnt->dimensions.x-1;
	playerEnt->position2.y = playerEnt->position.y+playerEnt->dimensions.y-1;
	playerEnt->speed = 5;
	playerEnt->numSpirits = 0;
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
	playerData->confidence = 20;
	playerData->maxConfidence = 30;
	playerData->rescuedSpirits = 0;
	playerData->guidingSpirits = 0;
	playerData->abandonSpirits = 0;
	

	playerEnt->update = &UpdatePlayer;
	playerEnt->draw = &DrawPlayer;
	playerEnt->think = &ThinkPlayer;
	playerEnt->touch = &TouchPlayer;

	SetElement(l,playerEnt);
	Insert(SpiritLink,playerEnt);
	Next(SpiritLink);
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
	RenderHPBar( 100, 0, -100,50,playerData->confidence/playerData->maxConfidence, hp ,hp2);
	if(playerData->confidence <= 0)
		printf("Player is Dead");
	ent->spiritIndex = 0;
	ent->hitBox.x =ent->position.x;
	ent->hitBox.y =ent->position.y;
	
	if(ent->velocity.x != 0 || ent->velocity.y != 0)
		ent->currentAnimation= 1;
	else
		ent->currentAnimation = 0;
	Vec2DAdd(ent->position,ent->position,OverlapsMap(map,ent));
	Vec2DAdd(ent->position,ent->position,ent->velocity);
}
void ThinkPlayer(Entity *ent)
{
	ent->numSpirits = SpiritLink->count;
	EntityIntersectAll(ent);
	Vec2DAdd(ent->position,ent->position,OverlapsMap(map,ent));
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
void RenderHPBar(int x, int y, int w, int h, float Percent, SDL_Color FGColor, SDL_Color BGColor) { 
   Percent = Percent > 1.f ? 1.f : Percent < 0.f ? 0.f : Percent; 
   SDL_Color old; 
   SDL_GetRenderDrawColor(GetRenderer(), &old.r, &old.g, &old.g, &old.a); 
   SDL_Rect bgrect = { x, y, w, h }; 
   SDL_SetRenderDrawColor(GetRenderer(), BGColor.r, BGColor.g, BGColor.b, BGColor.a); 
   SDL_RenderFillRect(GetRenderer(), &bgrect); 
   SDL_SetRenderDrawColor(GetRenderer(), FGColor.r, FGColor.g, FGColor.b, FGColor.a); 
   int pw = (int)((float)w * Percent); 
   int px = x + (w - pw); 
   SDL_Rect fgrect = { px, y, pw, h }; 
   SDL_RenderFillRect(GetRenderer(), &fgrect); 
   SDL_SetRenderDrawColor(GetRenderer(), old.r, old.g, old.b, old.a); 
} 