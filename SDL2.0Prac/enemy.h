#ifndef __ENEMY_H_
#define __ENEMY_H_

#include "spirit.h"

typedef struct{
	Entity *self;				/**< Pointer to Self entity*/
	EnemyState state;			/**< State of enemy*/
	int EnemyType;				/**< 0 for Lurker, 1 for Ghost*/
}Enemy;

Entity* CreateEnemy(int x, int y, int type);
void DrawEnemy(Entity *ent);
void UpdateEnemyChaser(Entity *ent);
void ThinkEnemyChaser(Entity *ent);
void UpdateEnemyLurker(Entity *ent);
void ThinkEnemyLurker(Entity *ent);
void TouchEnemy(Entity *ent, Entity *other);
void FreeEnemy(Entity *ent);
void SetEnemyAnimations(Entity *ent,char *filename);



#endif