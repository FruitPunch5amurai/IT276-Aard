#ifndef __PLAYER_H_
#define __PLAYER_H_
#include "entity.h"

typedef struct{
	int cooldown;
	int maxCooldown;
	int inuse;

}Ability;

typedef struct PlayerData{
	int confidence;
	int maxConfidence;
	int rescuedSpirits;
	int guidingSpirits;
	int abandonSpirits;
	Ability abilities[4];
	SDL_Rect* camera;

}Player;
void CreatePlayer();
Entity *GetPlayer();
void DrawPlayer(Entity* ent);
void UpdatePlayer(Entity* ent);
void ThinkPlayer(Entity *ent);
void TouchPlayer(Entity *ent,Entity *other);
void FreePlayer(Player* player);
void SetPlayerAnimations(Entity* ent);



#endif _PLAYER_H_
