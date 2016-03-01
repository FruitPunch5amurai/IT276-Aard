
#ifndef __ENTITY_H_
#define __ENTITY_H_


#include "level.h"
#include "sprite.h"
#include "vectors.h"
/**
*@brief Core data of the entity system
*/
enum GuidedState{
	BeingGuided,
	Lost,
	Untouched,
	Immunity

};
typedef struct EntityData
{
	int		inuse;
	int		id;
	int		whatAmI;
	int		speed;
	int		maxSpeed;
	SDL_Rect hitBox;
	Vec2D	position;
	Vec2D	position2;
	Vec2D	velocity;
	Vec2D	dimensions;
	Sprite*	sprite;
	int entType;
	int	currentAnimation;
	int entityListNum;
	SDL_RendererFlip flipped;
	int amIFlipped;
	SDL_Rect* camera;
	
	//Spirit stuff
	int spiritState;
	Uint32 nextThink;
	Vec2D savedPlayerPosition;
	int spiritIndex;
	Vec2D offset;
	
	void (*update)(struct EntityData *ent);
	void (*think)(struct EntityData *ent);
	int nextthink;
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
bool OverlapsMap(Map *map,Entity *ent);

int EntityIntersect(Entity *a, Entity *b);
Entity* EntityIntersectAll(Entity *a);
#endif 