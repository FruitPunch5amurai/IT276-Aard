#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <glib.h>
#include "level.h"
#include "vectors.h"
#include "sprite.h"
#include "graphics.h"
#include "editor_workspace.h"
#include "editor_tileselector.h"
extern Workspace* workSpace;

/*
*@brief Creates the TileSelector
*@param SDL_Rect which is the dimensions and location
*@return a pointer to the TileSelector
*/
TileSelector* CreateTileSelector(SDL_Rect pos)
{
	TileSelector *t;
	Button* button;
	Label* label;
	t= (TileSelector*)malloc(sizeof(TileSelector));
	memset(t,0,sizeof(TileSelector));
	t->position = pos;
	t->frameNum = 1;
	t->SpriteSheetListIndex = 0;
	t->SpriteSheet = NULL;
	t->buttons = NULL;
	t->visible =1;

	label = CreateEditorLabel("1",CreateSDL_Rect(pos.x + 10,pos.y + 35,16,16));
	label->color.r = 255;
	button = CreateEditorButton("images/ButtonLeft.png",CreateSDL_Rect(pos.x - 25,pos.y,16,16));
	button->function= &DecrementFrameNumber;
	button->data = label->text;
	t->buttons= g_list_append(t->buttons,button);

	button = CreateEditorButton("images/ButtonRight.png",CreateSDL_Rect(pos.x + 40,pos.y,16,16));
	button->function= &IncrementFrameNumber;
	button->data = label->text;
	t->label = label;
	t->buttons= g_list_append(t->buttons,button);

	return t;
}
/*
*@brief Updates a tile selector
*/
void UpdateTileSelector()
{
	if(workSpace->tileSelector->visible == 1)
		UpdateEditorButtons(workSpace->tileSelector->buttons);
}