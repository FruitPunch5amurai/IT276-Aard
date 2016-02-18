
#ifndef __ENTITY_H_
#define __ENTITY_H_

#include "sprite.h"
#include "vectors.h"
/**
*@brief Core data of the entity system
*/

typedef struct EntityData
{
	int		inuse;
	struct EntityData* ent;
	SDL_Rect hitBox;
	Vec2D	position;
	Vec2D	velocity;
	Vec2D	dimensions;
	Sprite*	sprite;
	int	currentAnimation;
	int entityListNum;
	SDL_RendererFlip flipped;
	int amIFlipped;

	void (*update)(struct EntityData *ent);
	void (*think)(struct EntityData *ent);
	int nextthink;
	int thinkRate;
	void (*touch)(struct EntityData *ent, struct EntityData *other);
	void (*free)(struct EntityData *ent);
	void(*draw)(struct EntityData *ent);
}Entity;

void DrawEntity(Entity *ent,int animationNum, int x, int y);	
void UpdateNone(Entity* ent);

Entity* CreateEntity();
void FreeEntity(Entity* ent);
void UpdateEntities();
void ThinkEntities();
void DrawEntities(); 
void InitEntityList();
void CloseEntityList();

#endif 