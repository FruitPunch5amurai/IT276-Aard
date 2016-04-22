#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <glib.h>
#include "graphics.h"
#include "editor_panel.h"
#include "editor.h"

extern GList* MainEditorPanels;
SDL_Color BG_Color;
/*
@brief Creates the initial panels
*/
void Init_Editor()
{
	CreateTopPanels();
}

void CreateTopPanels()
{
	Editor_Panel* Top;
	Editor_Panel* Panel1,* Panel2,* Panel3;
	Button* MakeRoom;
	SDL_Rect p1;
	p1.x = p1.y = 1;
	p1.w = 800;
	p1.h = 100;
	Top = CreateEditorPanel(p1);
	p1.w = 300;
	Panel1 = CreateEditorPanel(p1);
	p1.x +=300;
	Panel2 = CreateEditorPanel(p1);
	p1.x+=300;
	Panel3 = CreateEditorPanel(p1);
	MakeRoom = CreateEditorButton("images/CreateRoom.png",CreateSDL_Rect(700,85,32,16));
	Panel3->buttons = g_list_append(Panel3->buttons,MakeRoom);

	Top->panels = g_list_append(Top->panels,Panel1);
	Top->panels = g_list_append(Top->panels,Panel2);
	Top->panels = g_list_append(Top->panels,Panel3);
	MainEditorPanels = g_list_append(MainEditorPanels,Top);
	BG_Color.r = 100;BG_Color.b = 0;BG_Color.g = 0;
}
void DrawEditorButtons(GList* buttons)
{
	GList *elem;
	Button *ref;
	SDL_Rect src;
	int n;
		if(g_list_length(buttons) != 0){
		for(n = 0; n < g_list_length(buttons);++n)
		{
			elem = g_list_nth(buttons,n);
			ref = (Button*)elem->data;
			src.x = 0;
			src.y = 0;
			src.w =ref->sprite->w;
			src.h =ref->sprite->h;
			SDL_RenderCopyEx(GetRenderer(), ref->sprite->image,&src,&ref->rect,0,0,SDL_FLIP_NONE);
		}

	}
}
void DrawEditorPanels(GList* panels)
{
	GList *elem,*elem2;
	Editor_Panel* ref,*ref2;
	int n,m;
	if(g_list_length(panels) != 0){
		for(n = 0; n < g_list_length(panels);++n)
		{
			elem = g_list_nth(panels,n);
			ref = (Editor_Panel*)elem->data;
			SDL_SetRenderDrawColor(GetRenderer(), BG_Color.r, BG_Color.g, BG_Color.b, BG_Color.a); 
			SDL_RenderDrawRect(GetRenderer(), &ref->rect);
			SDL_SetRenderDrawColor(GetRenderer(), BG_Color.g, BG_Color.g, BG_Color.b, BG_Color.a); 
			if(g_list_length(ref->panels) != 0){
				for(m = 0; m < g_list_length(ref->panels);++m)
				{
					elem2 = g_list_nth(ref->panels,m);
					ref2 = (Editor_Panel*)elem2->data;
					SDL_SetRenderDrawColor(GetRenderer(), BG_Color.r, BG_Color.g, BG_Color.b, BG_Color.a); 
					SDL_RenderDrawRect(GetRenderer(), &ref2->rect);
					SDL_SetRenderDrawColor(GetRenderer(), BG_Color.g, BG_Color.g, BG_Color.b, BG_Color.a); 
					DrawEditorButtons(ref2->buttons);

				}
			//g_list_foreach(ref->panels,(GFunc)DrawEditorPanels,ref->panels);
			
		}
	}
	}
}
