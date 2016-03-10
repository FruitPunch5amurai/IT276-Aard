#ifndef __LLIST_H_
#define __LLIST_H_

#include "entity.h"
typedef struct LinkData Link;
typedef struct LinkData{
	Entity* curr;
	Link* next;
	Link* prev;

}Link;
Link* CreateLink(Entity* ent, Link* next, Link* prev);
Link* Next(Link* l);
void SetNext(Link* l, Link* l2);
Link* Prev(Link* l);
void SetPrev(Link* l, Link* l2);
Entity* Element(Link* l);
void SetElement(Link* l, Entity* ent);

typedef struct{
	Link* head;
	Link* tail;
	Link* curr;
	int count;
	int maxCount;
}ELink;
ELink* CreateELink();
void Clear(ELink* e);
void Append(ELink* e, Entity* ent);
void Insert(ELink* e,Entity* ent);
int moveToPos(ELink* e,int pos);
void moveToEnd(ELink* e);
void moveToStart(ELink* e) ;
Entity* Remove(ELink* e);
void Prev(ELink* e);
void Next(ELink* e);

#endif
