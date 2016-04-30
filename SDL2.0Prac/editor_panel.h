#ifndef __EDITOR_PANEL_H_
#define __EDITOR_PANEL_H_

#include "sprite.h"
#include "entity.h"
#include "gamepad.h"
#include "game.h"
/*
*@brief Structures for Map Editor, Supports a two tier panel system. Each Structures functions 
*@brief are below each structure
*/
typedef struct ButtonData Button;
typedef struct Editor_PanelData Editor_Panel;
typedef struct ButtonData{
	Sprite* sprite;
	SDL_Rect rect;
	Editor_Panel* parentPanel;
	char name[255];
	int clicked;

	void *data;

	void (*listen)();
	void (*function)(Button* button);
}Button;
void UpdateEditorButton();
Button* CreateEditorButton(char *text,SDL_Rect rect);
void FreeButton(Button* button);
void DecrementFrameNumber(Button* button);
void IncrementFrameNumber(Button* button);

typedef struct Editor_PanelData Editor_Panel;
typedef struct {
	char name[255];
	SDL_Rect rect;
	char text[255];
	Editor_Panel *parentPanel;

}Label;
Label* CreateEditorLabel(char* text,SDL_Rect rect);
void FreeLabel(Label* label);
typedef struct
{
	char value[255];
	char name[255];
	TTF_Font *font;
	SDL_Rect rect;
	int focus;


}TextBox;
TextBox* CreateEditorTextBox(SDL_Rect rect);
TextBox* UpdateEditorTextBoxes(GList* texts);
void DrawEditorTextBoxes(GList* texts);
void FreeTextBox(TextBox* text);
typedef struct Editor_PanelData {
	int focus;
	int visible;
	char name[255];
	SDL_Rect rect;
	GList* siblingPanels;
	GList* panels;
	GList* labels;
	GList* buttons;
	GList* texts;
	
}Editor_Panel;
void UpdateEditorPanel(GList *panels);
void FreeEditorPanel(Editor_Panel* panel);
Editor_Panel* CreateEditorPanel(SDL_Rect rect);
void SetCurrentPanel();
void RemovePanel(Editor_Panel* panel);
typedef struct
{
	GList *buttons;
	SDL_Rect position;
	GList* SpriteList;
	Sprite *SpriteSheet;
	int SpriteSheetListIndex;
	int frameNum;

}TileSelector;
void DrawCurrentSpriteTile();
TileSelector* CreateTileSelector(SDL_Rect pos);
void UpdateTileSelector();

typedef struct {
	Map* map;
	char filename[255];
	TileSelector *tileSelector;
	int focus;
	SDL_Texture *buffer;
	int activeLayer;
}Workspace;
Workspace* CreateEditorWorkspace();
void DrawWorkspace();
/*
*@brief Functions for loading maps
*/
void LoadEditorMapPanel(Button* button);
void LoadEditorMapNew(Button* button);
void LoadEditorMap(Button* button);

/*
*@brief Functions for lFreeing
*/
void FreeWorkSpace();
void FreeEveryThing();

/*
*@brief misc
*/
void UpdateMousePosition();
void LoadSpriteSheet(Button* button);
void ChangeSpriteSheet(Button* button);
SDL_Rect* GetMousePosition();
SDL_Rect CreateSDL_Rect(int x,int y,int w,int h);

extern GList* MainEditorPanels;
#endif
