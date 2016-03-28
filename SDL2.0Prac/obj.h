#ifndef _OBJ_H_
#define _OBJ_H_





void CreateObject(int x, int y,int width, int height, int type);
void ThinkObject(Entity* ent);
void TouchObject(Entity* ent,Entity* other);
void UpdateObject(Entity* ent);
void DrawObject(Entity* ent);
void BreakObject(Entity* ent);


#endif