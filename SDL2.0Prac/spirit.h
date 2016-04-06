#ifndef _SPIRIT_H_
#define _SPIRIT_H_
#include "entity.h"



Entity* CreateSpirit(int x,int y);
void DrawSpirit(Entity* ent);
void UpdateSpirit(Entity* ent);
void ThinkSpirit(Entity *ent);
void TouchSpirit(Entity *ent,Entity *other);
void FreeSpirit(Entity* ent);
void ClearSpiritLink();



#endif

