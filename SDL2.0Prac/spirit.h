#ifndef _SPIRIT_H_
#define _SPIRIT_H_



typedef struct SpiritData{
	int isBeingGuided;
	int speed;
	int inuse;
	Entity *next;
	Entity *prev;



}Spirit;

Entity* CreateSpirit(int x,int y);
void DrawSpirit(Entity* ent);
void UpdateSpirit(Entity* ent);
void ThinkSpirit(Entity *ent);
void TouchSpirit(Entity *ent,Entity *other);
void FreeSpirit(Spirit* spirit);


#endif

