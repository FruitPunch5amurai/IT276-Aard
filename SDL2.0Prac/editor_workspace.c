#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <glib.h>
#include "level.h"
#include "vectors.h"
#include "graphics.h"
#include "editor_panel.h"
#include "editor_workspace.h"
extern SDL_Event* mainEvent;
Workspace* workSpace;
extern Editor_Panel* currentPanel;
/*
*@brief Creates the Workspace
*@return a pointer to the Workspace
*/
Workspace* CreateEditorWorkspace()
{
	Workspace *ws;
	ws = (Workspace*)malloc(sizeof(Workspace));
	memset(ws,0,sizeof(Workspace));
	ws->map = NULL;
	ws->activeLayer = 1;
	ws->mode = Add;
	return ws;
}

/*
*@brief Updates the Workspace only if currentPanel is NULL.
*/
void UpdateWorkspace()
{
	int x,y;
	GList *elem;
	Sprite* ref;
	int frame = 0;
	if(workSpace->map != NULL){
		if(currentPanel == NULL)
		{
			SDL_GetMouseState(&x,&y);
			if(x > 750 && (workSpace->areaToDraw.x + workSpace->areaToDraw.w) < workSpace->map->w * workSpace->map->tileW )
			{
				workSpace->areaToDraw.x += 10;
			}
			else if(x < 50 && (workSpace->areaToDraw.x) > 0 )
			{
				workSpace->areaToDraw.x -= 10;
			}
			else if(y < 50 && (workSpace->areaToDraw.y) > 0)
			{
				workSpace->areaToDraw.y -= 10;
			}
			else if(y > 550 && (workSpace->areaToDraw.y + workSpace->areaToDraw.h) < workSpace->map->h * workSpace->map->tileH)
			{
				workSpace->areaToDraw.y += 10;
			}
			//Add and Remove Tiles
			if(mainEvent->type == SDL_MOUSEBUTTONUP && workSpace->mode == Add){
				if(workSpace->activeLayer == 1)
					ChangeTile(workSpace->map->data,
					(GetMousePosition()->x + workSpace->areaToDraw.x)/workSpace->map->tileW,
					(GetMousePosition()->y + workSpace->areaToDraw.y)/workSpace->map->tileH
					,workSpace->tileSelector->frameNum);
				if(workSpace->activeLayer == 2)
					ChangeTile(workSpace->map->data2,
					(GetMousePosition()->x + workSpace->areaToDraw.x)/workSpace->map->tileW,
					(GetMousePosition()->y + workSpace->areaToDraw.y)/workSpace->map->tileH
					,workSpace->tileSelector->frameNum);
				if(workSpace->activeLayer == 3)
					ChangeTile(workSpace->map->data3,
					(GetMousePosition()->x + workSpace->areaToDraw.x)/workSpace->map->tileW,
					(GetMousePosition()->y + workSpace->areaToDraw.y)/workSpace->map->tileH
					,workSpace->tileSelector->frameNum);

			}else if(mainEvent->type == SDL_MOUSEBUTTONUP && workSpace->mode == Remove)
			{
				if(workSpace->activeLayer == 1)
					ChangeTile(workSpace->map->data,
					(GetMousePosition()->x + workSpace->areaToDraw.x)/workSpace->map->tileW,
					(GetMousePosition()->y + workSpace->areaToDraw.y)/workSpace->map->tileH
					,0);
				if(workSpace->activeLayer == 2)
					ChangeTile(workSpace->map->data2,
					(GetMousePosition()->x + workSpace->areaToDraw.x)/workSpace->map->tileW,
					(GetMousePosition()->y + workSpace->areaToDraw.y)/workSpace->map->tileH
					,0);
				if(workSpace->activeLayer == 3)
					ChangeTile(workSpace->map->data3,
					(GetMousePosition()->x + workSpace->areaToDraw.x)/workSpace->map->tileW,
					(GetMousePosition()->y + workSpace->areaToDraw.y)/workSpace->map->tileH
					,0);
			}

		}
	}
}
void SwitchMode()
{
	workSpace->mode = workSpace->mode == Add?Remove:Add;
}
/*
*@brief Decreases the frame number for the TileSelector
*@param The Button which has been called
*/
void DecrementFrameNumber(Button* button)
{
	int x;
	if(workSpace->tileSelector->frameNum >1)
		workSpace->tileSelector->frameNum-=1;
	sprintf((char*)button->data,"%d",workSpace->tileSelector->frameNum);
	/*
	x = atoi((char*)button->data);
	x-=1;
	sprintf((char*)button->data,"%d",x);
	*/
}
void DecrementActiveLayer(Button* button)
{
	int x;
	x = atoi((char*)button->data);
	if(x == 1){
		x = 3;
		workSpace->activeLayer = 3;
	}
	else
		x-=1;
		workSpace->activeLayer -= 1;
	sprintf((char*)button->data,"%d",x);

}
void IncrementActiveLayer(Button* button)
{
	int x;
	x = atoi((char*)button->data);
	if (x == 3){
		x = 1;
		workSpace->activeLayer =1;
	}
	else{
		x+=1;
		workSpace->activeLayer += 1;
	}
	sprintf((char*)button->data,"%d",x);
}
	
/*
*@brief Increases the frame number for the TileSelector
*@param The Button which has been called
*/
void IncrementFrameNumber(Button* button)
{
	int x;
	x = atoi((char*)button->data);
	workSpace->tileSelector->frameNum+=1;
	sprintf((char*)button->data,"%d",workSpace->tileSelector->frameNum);
}