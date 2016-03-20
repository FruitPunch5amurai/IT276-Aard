#ifndef __PLAYER_H_
#define __PLAYER_H_
#include "entity.h"
	

typedef struct{
	int cooldown;
	int maxCooldown;
	int inuse;
	int unlocked;

}Ability;

typedef struct PlayerData{
	int confidence;
	int maxConfidence;
	int EXP;
	int rescuedSpirits;
	int guidingSpirits;
	int abandonSpirits;
	TTF_Font *font;
	SDL_Rect textRect;
	Ability abilities[4];
	SDL_Rect* camera;

}Player;
void CreatePlayer(int x,int y);
Entity *GetPlayer();
void DrawPlayer(Entity* ent);
void UpdatePlayer(Entity* ent);
void ThinkPlayer(Entity *ent);
void TouchPlayer(Entity *ent,Entity *other);
void FreePlayer();
void SetPlayerAnimations(Entity* ent);
void RenderHPBar(int x, int y, int w, int h, float Percent, SDL_Color FGColor, SDL_Color BGColor) ;
void UpdateGUI();
void ExecuteSkill();
void SkillWhip();
void SkillRetrieve();
void SkillNEVERGONNAGIVEYOUUP();

#endif _PLAYER_H_
