#ifndef __PLAYER_H_
#define __PLAYER_H_
#include "entity.h"
#include "items.h"

typedef struct{
	int cooldown;
	int maxCooldown;
	int inuse;
	int unlocked;
	int dmg;

}Ability;

typedef struct PlayerData{
	int confidence;
	int maxConfidence;
	int EXP;
	int rescuedSpirits;
	int guidingSpirits;
	int abandonSpirits;
	SDL_Rect textRect;
	Ability abilities[4];
	SDL_Rect* camera;
	Inventory* inventory;
	Items *currentItem;

}Player;
void CreatePlayerData();
void SetPlayerData();
void CreatePlayer(int x,int y);
Entity *GetPlayer();
void DrawPlayer(Entity* ent);
void UpdatePlayer(Entity* ent);
void ThinkPlayer(Entity *ent);
void TouchPlayer(Entity *ent,Entity *other);
void FreePlayer(Entity* ent);
void SetPlayerAnimations(Entity* ent);
void RenderHPBar(int x, int y, int w, int h, float Percent, SDL_Color FGColor, SDL_Color BGColor) ;
void UpdateGUI();
void ExecuteSkill();
void UseItem();
void SkillWhip();
void SkillRetrieve();
void SkillNEVERGONNAGIVEYOUUP();
void SkillPickUpObject(Entity* ent);
void SkillThrowObject();
void TransitionRoom(Entity* ent,Room* room);
void AddSpiritToPlayer();
void RespawnPlayer();
void CheckPitFalls(Map *map,Entity *ent);

#endif _PLAYER_H_
