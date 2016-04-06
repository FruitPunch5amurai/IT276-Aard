#ifndef __ENEMY_H_
#define __ENEMY_H_

#include "spirit.h"

Entity* CreateEnemy(int x, int y, EnemyType type);
void DrawEnemy(Entity *ent);
void UpdateEnemyChaser(Entity *ent);
void ThinkEnemyChaser(Entity *ent);
void UpdateEnemyLurker(Entity *ent);
void ThinkEnemyLurker(Entity *ent);
void UpdateEnemySnatcher(Entity *ent);
void ThinkEnemySnatcher(Entity *ent);
void TouchEnemy(Entity *ent, Entity *other);
void FreeEnemy(Entity *ent);
void SetEnemyAnimations(Entity *ent,char *filename);



#endif