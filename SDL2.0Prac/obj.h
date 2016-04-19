#ifndef _OBJ_H_
#define _OBJ_H_
#include "items.h"



Entity* CreateObject(Vec2D position,int width, int height, int type,int frame,
	char* filename, ItemRef *item);
void ThinkObject(Entity* ent);
void TouchObject(Entity* ent,Entity* other);
void UpdateObject(Entity* ent);
void DrawObject(Entity* ent);
void BreakObject(Entity* ent);
;


#endif