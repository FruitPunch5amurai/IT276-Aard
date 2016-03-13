
#ifndef __ENTITY_H_
#define __ENTITY_H_


#include "level.h"
#include "sprite.h"
#include "vectors.h"
#include "particle.h"
/**
*@brief Core data of the entity system
*/
typedef struct EntityData Entity;
enum EnemyState{
	IDLE,
	MOVING,
	CHASING,
	GOINGUP,
	GOINGDOWN
};

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
	int		whatAmI;		/**< 0 is Aard, 1 is Spirit, 2 is Enemy, 3 is Portal*/
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
	SDL_RendererFlip flipped;
	int amIFlipped;
	SDL_Rect* camera;
	int numSpirits;

	//Spirit stuff
	int spiritState;
	int isBeingGuided;
	Uint32 nextThink;
	Vec2D  savedPlayerPos;
	int spiritIndex;	
	Vec2D offset;
	Entity* follow;
	Particle *particles;
	//Enemy Stuff
	EnemyState state;			/**< State of enemy*/
	int enemyType;				/**< 0 for Lurker, 1 for Ghost,2 for Chaser*/
	int nextMove;
	Uint32 temp;

	void (*update)(struct EntityData *ent);
	void (*think)(struct EntityData *ent);
	void (*touch)(struct EntityData *ent, struct EntityData *other);
	void (*free)(struct EntityData *ent);
	void(*draw)(struct EntityData *ent);
}Entity;

void DrawEntity(Entity *ent,int animationNum, int x, int y);	

Entity* CreateEntity();
void FreeEntity(Entity* ent);
void UpdateEntities();
void ThinkEntities();
void DrawEntities(); 
void InitEntityList();
void CloseEntityList();
Vec2D OverlapsMap(Map *map,Entity *ent);

int EntityIntersect(Entity *a, Entity *b);
Entity* EntityIntersectAll(Entity *a);
int GetID(Entity *ent);
Entity* GetEntityByID(int id);
Entity *CreatePortal(int x, int y);
void DrawPortal(Entity *ent);



#endif 