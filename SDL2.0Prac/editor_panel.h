#ifndef __EDITOR_PANEL_H_
#define __EDITOR_PANEL_H_

#include "sprite.h"
#include "entity.h"

typedef struct {
	Sprite* sprite;
	SDL_Rect rect;
	int clicked;

	void *data;

	void (*listen)();
	void (*function)();
}Button;
typedef struct {
	SDL_Rect rect;
	char text[255];

}Label;
typedef struct {
	int focus;
	SDL_Rect rect;
	GList* panels;
	GList* labels;
	GList* buttons;
	GList* texts;
	
}Editor_Panel;
typedef struct {
	Map* map;
	char filename[255];
	int activeLayer;
}Workspace;
Label* CreateEditorLabel(char text,SDL_Rect rect);
Button* CreateEditorButton(char *text,SDL_Rect rect);
Editor_Panel* CreateEditorPanel(SDL_Rect rect);
Workspace* CreateEditorWorkspace();


SDL_Rect CreateSDL_Rect(int x,int y,int w,int h);

extern GList* MainEditorPanels;
#endif
