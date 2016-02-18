#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "sprite.h"
#include "vectors.h"
#include "player.h"

Player *player;

/**
*@brief Creates and Allocates memory for the player. Then initializes all the
*@brief data members for Player
*/
Player* CreatePlayer()
{
	player = (Player*)malloc(sizeof(Player));
	memset(player,0,sizeof(player));
	player->self = CreateEntity();
	
	//Data for Idle Animation
	player->self->sprite= LoadSprite("images/Sonic.png",32,42);
	player->self->sprite->animation[0]->startFrame = 5;
	player->self->sprite->animation[0]->maxFrames = 5;
	player->self->sprite->animation[0]->oscillate = true;

	//Data for Run Animation
	player->self->sprite->animation[1]->maxFrames = 6;
	player->self->sprite->animation[1]->oscillate = true;
	player->self->sprite->animation[1]->startFrame = 17;
	player->self->sprite->animation[1]->currentFrame = 17;

	
	player->playerSpeed = 2;
	player->self->update = &UpdatePlayer;
	player->self->draw = &DrawPlayer;
	player->self->think = &ThinkPlayer;
	player->self->touch = &TouchPlayer;

	return player;
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
	ent->currentAnimation= 1;	
	Vec2DAdd(ent->position,ent->position,ent->velocity);
	DrawPlayer(ent);
}
void ThinkPlayer(Entity *ent)
{

}
void TouchPlayer(Entity *ent,Entity *other)
{

}
/**
*@brief Deallocates memory for player
*/
void FreePlayer(Player* player)
{
	FreeEntity(player->self);
	free(player);
	player = NULL;
}
/**
*@brief Loads in Data for AnimationData for the player from a .txt
*/
void LoadPlayerAnimations(Player* player)
{
	
}