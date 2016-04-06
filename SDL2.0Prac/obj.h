#ifndef _OBJ_H_
#define _OBJ_H_





Entity* CreateObject(Vec2D position,int width, int height, int type,int frame);
void ThinkObject(Entity* ent);
void TouchObject(Entity* ent,Entity* other);
void UpdateObject(Entity* ent);
void DrawObject(Entity* ent);
void BreakObject(Entity* ent);
;


#endif