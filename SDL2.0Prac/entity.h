
#ifndef __ENTITY_H_
#define __ENTITY_H_

#include "sprite.h"

typedef struct EntityData
{
	int		inuse;
	struct EntityData* ent;
	SDL_Rect hitBox;
//	Vec2d	position;
//	Vec2d	velocity;
	int w,h;
	Sprite*	sprite;
	//int		frame;
	int entityListNum;

	void (*update)(struct EntityData *ent);
	void (*think)(struct EntityData *ent);
}Entity;

void Draw(Entity *ent,int animationNum);	
void UpdateNone(Entity* ent);

Entity* CreateEntity();
void FreeEntity();
void UpdateEntities();
void ThinkEntities();
void InitEntityList();
void CleanupEntityList();

#endif 