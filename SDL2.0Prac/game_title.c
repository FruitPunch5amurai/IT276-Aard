#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <glib.h>
#include <string>
#include "graphics.h"
#include "sprite.h"
#include "player.h"
#include "save.h"
#include "editor.h"
#include "editor_panel.h"
#include "game_title.h"

extern GList *MainEditorPanels;
extern Player *playerData;
extern SDL_Event* mainEvent;
extern int ButtonDown;
void SetUpTitle()
{
	Button* NewGame, *Continue,*Editor;
	Editor_Panel* panel;
	Label* Title;
	NewGame = CreateEditorButton(NULL,CreateSDL_Rect(100,200,255,100));
	Continue = CreateEditorButton(NULL,CreateSDL_Rect(300,330,255,100));
	Editor = CreateEditorButton(NULL,CreateSDL_Rect(500,460,255,100));
	
	NewGame->sprite = LoadSprite("images/New_Game_Button.png",255,100);
	NewGame->sprite->fpl = 1;
	strcpy(NewGame->name,"NewGame");
	Continue->sprite = LoadSprite("images/Continue_Button.png",255,100);
	Continue->sprite->fpl = 1;
	strcpy(Continue->name,"Continue");
	Editor->sprite = LoadSprite("images/Level_Editor_Button.png",255,100);
	Editor->sprite->fpl = 1;
	strcpy(Editor->name,"Editor");
	Title = CreateEditorLabel(NULL,CreateSDL_Rect(0,0,350,200));
	Title->sprite = LoadSprite("images/Aard_Title.png",350,200);
	
	panel = CreateEditorPanel(CreateSDL_Rect(0,0,800,600));
	panel->buttons = g_list_append(panel->buttons,NewGame);
	panel->buttons = g_list_append(panel->buttons,Continue);
	panel->buttons = g_list_append(panel->buttons,Editor);
	panel->labels = g_list_append(panel->labels,Title);
	MainEditorPanels = g_list_append(MainEditorPanels,panel);
	
	atexit(FreeEveryThing);

}
void DrawTitle()
{
	GList *elem,*elem2;
	Button *ref;
	SDL_Rect src;
	Editor_Panel* panel;
	Label* label;
	int n,i,j;
	if(game->gameState == &StateTitle){
		if(g_list_length(MainEditorPanels) != 0){
		for(n = 0; n < g_list_length(MainEditorPanels);++n)
		{
			elem = g_list_nth(MainEditorPanels,n);
			panel = (Editor_Panel*)elem->data;
			for(i = 0; i < g_list_length(panel->buttons);++i)
			{
				elem2 = g_list_nth(panel->buttons,i);
				ref = (Button*)elem2->data;
				DrawSprite(ref->sprite,ref->rect.x,ref->rect.y,ref->frame,GetRenderer(),SDL_FLIP_NONE);
			}
			for(i = 0; i < g_list_length(panel->labels);++i)
			{
				elem2 = g_list_nth(panel->labels,i);
				label = (Label*)elem2->data;
				DrawSprite(label->sprite,label->rect.x,label->rect.y,0,GetRenderer(),SDL_FLIP_NONE);
			}

		}
		}
	}
}

void UpdateTitleButtons()
{
	GList *elem,*elem2;
	Button *ref;
	Editor_Panel* panel;
	int n,i,j;
	int x,y;
		if(g_list_length(MainEditorPanels) != 0){
		for(n = 0; n < g_list_length(MainEditorPanels);++n)
		{
			elem = g_list_nth(MainEditorPanels,n);
			panel = (Editor_Panel*)elem->data;
			for(i = 0; i < g_list_length(panel->buttons);++i)
			{
				elem2 = g_list_nth(panel->buttons,i);
				ref = (Button*)elem2->data;

				SDL_GetMouseState(&x,&y);
				if( x > ref->rect.x && x < (ref->rect.x +  ref->rect.w) 
					&& y >  ref->rect.y &&   y <  (ref->rect.y +  ref->rect.h))
				{
					if(ref->inside == 0)
						Mix_PlayChannel (1,ref->soundEffect, 0);
				
				    ref->inside = 1;
					ref->frame = 1;
				}
				else{
					ref->inside = 0;
					ref->frame = 0;
				}
				if(ref->inside == true){
				printf("Mouse inside a button..");
				if(mainEvent->type == SDL_MOUSEBUTTONDOWN && ButtonDown == 0)
				{
					printf("Button Executed\n");
					if(strcmp(ref->name,"NewGame") == 0)
					{
						FreeEveryThing();
						CreateNewPlayerSave("playerData");
						PlayerLoad("playerData");
						SetGameState(StateGame);
						
					}
					else if(strcmp(ref->name,"Continue") == 0)
					{
						FreeEveryThing();
						PlayerLoad("playerData");
						SetGameState(StateGame);
					}
					else if(strcmp(ref->name,"Editor") == 0)
					{
						FreeEveryThing();
						SetGameState(StateEditor);
					}
					return;
				}
			
			}
			}
		}

	}
}

