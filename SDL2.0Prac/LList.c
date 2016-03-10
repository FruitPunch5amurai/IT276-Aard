#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "LList.h"

Link* CreateLink(Entity* ent, Link* next, Link* prev)
{
	Link *l;
	l = (Link*)malloc(sizeof(Link));
	memset(l,0,sizeof(Link));
	l->curr = ent;
	l->next = next;
	l->prev = prev;
	return l;
}

Link* Next(Link* l)
{
	return l->next;
}
void SetNext(Link* l, Link* l2)
{
	l->next = l2;
}

Link* Prev(Link* l)
{
		return l->prev;
}
void SetPrev(Link* l, Link* l2 )
{
	l->prev = l2;
}

Entity* Element(Link* l)
{
	return l->curr;
}
void SetElement(Link* l, Entity* ent)
{
	l->curr = ent;
}
ELink* CreateELink()
{
	ELink *e;
	e = (ELink*)malloc(sizeof(ELink));
	memset(e,0,sizeof(ELink));
	e->curr = e->head = CreateLink(NULL,NULL,NULL);
	e->tail = CreateLink(NULL,e->head,NULL);
	SetNext(e->head,e->tail);
	e->count = 0;
	return e;
}
void Clear(ELink* e)
{
	SetNext(e->head,NULL);
	e->curr = e->head = CreateLink(NULL,NULL,NULL);
	e->tail = CreateLink(NULL,e->head,NULL);
	SetNext(e->head,e->tail);
	e->count = 0;
}
int moveToPos(ELink* e,int pos)
{
	if(pos < 0 || pos > e->count)
	{
		printf("Out of Bounds Index");
		return 0;
	}else
	{
		e->curr = e->head;
		for(int i=0; i<pos; i++) e->curr = e->curr->next;
		return 1;
	}
}
void Append(ELink* e, Entity* ent)
{
	SetPrev(e->tail,CreateLink(ent,Prev(e->tail),e->tail));
	if(Prev(Prev(e->tail)) != NULL)
		SetNext(Prev(Prev(e->tail)),Prev(e->tail));
	e->count++;
}

void Insert(ELink* e,Entity* ent)
{
	SetNext(e->curr,CreateLink(ent,e->curr->next,e->curr));
	SetPrev(e->curr->next->next,e->curr->next);
	e->count++;
}
Entity* Remove(ELink* e)
{

	Entity* ent = Next(e->curr)->curr;
	SetNext(Prev(e->curr),Next(e->curr));
	SetPrev(Next(e->curr),Prev(e->curr));
	e->curr = Prev(e->curr);
	e->count--;

	return ent;
}
//Moving Curr
void moveToEnd(ELink* e)
{
	e->curr = e->tail->prev;
}
 void moveToStart(ELink* e) 
{
	e->curr = e->head; 
}
void Prev(ELink* e)
{
	if(e->curr != e->head)
		e->curr = Prev(e->curr);
}
void Next(ELink* e)
{
	if(e->curr != Prev(e->tail))
		e->curr = Next(e->curr);
}