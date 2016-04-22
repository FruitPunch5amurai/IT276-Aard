
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <glib.h>
#include "editor_panel.h"

GList* MainEditorPanels = NULL;

Editor_Panel* CreateEditorPanel(SDL_Rect rect)
{
	Editor_Panel* p;
	p = (Editor_Panel*)malloc(sizeof(Editor_Panel));
	memset(p,0,sizeof(Editor_Panel));
	p->panels = NULL;
	p->texts = NULL;
	p->buttons = NULL;
	p->labels = NULL;
	p->rect = rect;
	p->focus = 0;
	return p;
}
Button* CreateEditorButton(char* text,SDL_Rect rect)
{
	Button *b;
	b = (Button*)malloc(sizeof(Button));
	memset(b,0,sizeof(Button));
	b->clicked = 0;
	b->function = NULL;
	b->rect = rect;
	b->listen = NULL;
	b->sprite = LoadSprite(text,32,16);							/*< for now */
	return b;
}
Label* CreateEditorLabel(char text,SDL_Rect rect)
{
	Label* l;
	l = (Label*)malloc(sizeof(Label));
	memset(l,0,sizeof(Label));
	strncpy(l->text,&text,255);
	l->rect = rect;
	return l;
}

Workspace* CreateEditorWorkspace()
{
	Workspace *ws;
	ws = (Workspace*)malloc(sizeof(Workspace));
	memset(ws,0,sizeof(Workspace));
	ws->activeLayer = 0;
	return ws;
}

SDL_Rect CreateSDL_Rect(int x,int y,int w,int h)
{
	SDL_Rect rect;
	rect.y = y;
	rect.x = x;
	rect.w = w;
	rect.h = h;
	return rect;
}