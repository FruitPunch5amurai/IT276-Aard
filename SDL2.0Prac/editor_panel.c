
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <glib.h>
#include "level.h"
#include "vectors.h"
#include "graphics.h"
#include "editor_panel.h"
extern Game* game;
extern SDL_Event *mainEvent;
extern Workspace* workSpace;
GList* MainEditorPanels = NULL;
Editor_Panel* currentPanel = NULL;
TextBox* currentTextBox = NULL;
SDL_Rect MousePosition;
int ButtonDown = 0;
Workspace* CreateEditorWorkspace()
{
	Workspace *ws;
	ws = (Workspace*)malloc(sizeof(Workspace));
	memset(ws,0,sizeof(Workspace));
	ws->activeLayer = 0;
	return ws;
}
TileSelector* CreateTileSelector(SDL_Rect pos)
{
	TileSelector *t;
	Button* button;
	t= (TileSelector*)malloc(sizeof(TileSelector));
	memset(t,0,sizeof(TileSelector));
	t->position = pos;
	t->frameNum = 0;
	t->SpriteSheetListIndex = 0;
	t->buttons = NULL;

	button = CreateEditorButton("images/ButtonLeft.png",CreateSDL_Rect(pos.x - 25,pos.y,16,16));
	button->function= &DecrementFrameNumber;
	button->data = &workSpace->tileSelector->frameNum;
	t->buttons= g_list_append(t->buttons,button);

	button = CreateEditorButton("images/ButtonRight.png",CreateSDL_Rect(pos.x + 40,pos.y,16,16));
	button->function= &IncrementFrameNumber;
	button->data = &workSpace->tileSelector->frameNum;
	t->buttons= g_list_append(t->buttons,button);

	return t;
}
Editor_Panel* CreateEditorPanel(SDL_Rect rect)
{
	Editor_Panel* p;
	p = (Editor_Panel*)malloc(sizeof(Editor_Panel));
	memset(p,0,sizeof(Editor_Panel));
	p->visible = 1;
	p->panels = NULL;
	p->texts = NULL;
	p->buttons = NULL;
	p->labels = NULL;
	p->rect = rect;
	p->focus = 0;
	return p;
}
Label* CreateEditorLabel(char* text,SDL_Rect rect)
{
	Label* l;
	l = (Label*)malloc(sizeof(Label));
	memset(l,0,sizeof(Label));
	strncpy(l->text,text,255);
	l->rect = rect;
	return l;
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
TextBox* CreateEditorTextBox(SDL_Rect rect)
{
	TextBox *b;
	b = (TextBox *)malloc(sizeof(TextBox ));
	memset(b,0,sizeof(TextBox ));
	b->focus = 0;
	b->rect = rect;
	strcpy(b->value ,"Enter Value Here");
	return b;
}

void UpdateEditorButtons(GList* buttons)
{
	GList *elem;
	Button *ref;
	SDL_Rect src;
	int x,y;
	bool inside = false;
	int n;
		if(g_list_length(buttons) != 0){
		for(n = 0; n < g_list_length(buttons);++n)
		{
			elem = g_list_nth(buttons,n);
			ref = (Button*)elem->data;
			if( mainEvent->type == SDL_MOUSEMOTION || mainEvent->type == SDL_MOUSEBUTTONDOWN){
				SDL_GetMouseState(&x,&y);
				if( x > ref->rect.x && x < (ref->rect.x +  ref->rect.w) 
					&& y >  ref->rect.y &&   y <  (ref->rect.y +  ref->rect.h))
				{
				    inside = true;
				}
				else
					inside = false;
			
			}if(inside == true){
				printf("Mouse inside a button..");
				if(mainEvent->type == SDL_MOUSEBUTTONDOWN && ButtonDown == 0)
				{
					printf("Button Executed\n");
				 	(*ref->function)(ref);
					ButtonDown = 1;
					return;
				}
			}
		}
	}if(mainEvent->type == SDL_MOUSEBUTTONUP && ButtonDown != 0)
				{
					ButtonDown = 0;
					return;
				}
}

TextBox* UpdateEditorTextBoxes(GList* texts)
{
	GList *elem;
	TextBox *ref;
	SDL_Rect src;
	int x,y;
	int focus = 0;
	int n;
	if(g_list_length(texts) != 0){
		for(n = 0; n < g_list_length(texts);++n)
		{
			elem = g_list_nth(texts,n);
			ref = (TextBox*)elem->data;
			if(ref->focus == 1){
				currentTextBox = ref;
				return ref;
			}
			if( GetMousePosition()->x > ref->rect.x && GetMousePosition()->x < (ref->rect.x +  ref->rect.w) 
					&& GetMousePosition()->y >  ref->rect.y &&   GetMousePosition()->y <  (ref->rect.y +  ref->rect.h))
			{
				printf("Mouse over a TextBox");
				if(mainEvent->type == SDL_MOUSEBUTTONDOWN)
								printf("Clicked a textbox");
				memset(ref->value,0,sizeof(char));
				ref->focus = 1;
				currentTextBox = ref;
				return ref;
				}

			}
		}
	return NULL;

	}
void UpdateTileSelector()
{
	UpdateEditorButtons(workSpace->tileSelector->buttons);
}
void UpdateEditorPanel(GList* panels)
{
	GList *elem,*elem2;
	Editor_Panel* ref,*ref2;
	TextBox* textBox;
	int n,m;
	UpdateTileSelector();
	if(currentPanel == NULL)
	{
		elem = g_list_last(MainEditorPanels);
		ref = (Editor_Panel*)elem->data;
		currentPanel = ref;
	}else
	{
		if(mainEvent->type == SDL_MOUSEBUTTONDOWN)
		{
			if(currentTextBox != NULL)
			{
			currentTextBox->focus = 0;
			currentTextBox = NULL;
			SDL_StopTextInput();
			}
		}
		if(g_list_length(currentPanel->panels) != 0){
				for(m = 0; m < g_list_length(currentPanel->panels);++m)
				{
					elem2 = g_list_nth(currentPanel->panels,m);
					ref2 = (Editor_Panel*)elem2->data;

					if(!currentTextBox)
						currentTextBox= UpdateEditorTextBoxes(ref2->texts);
					if(!currentTextBox)
					{
					UpdateEditorButtons(ref2->buttons);
					}else
					{
						SDL_StartTextInput();
						SDL_SetTextInputRect(&currentTextBox->rect);
						if(GetKeyData()->BackSpace == 1)
							{
								if(mainEvent->key.keysym.sym == SDLK_BACKSPACE && strlen(currentTextBox->value) > 0)
								{
									currentTextBox->value[strlen(currentTextBox->value)-1] = '\0';
									GetKeyData()->BackSpace = 2;
								}
							}
						while(SDL_PollEvent(mainEvent) != 0)
							{
							if(mainEvent->type == SDL_TEXTINPUT)
							{
								strcat(currentTextBox->value,mainEvent->text.text);
							}
						}

					}
				}
		}else
		{
					if(!currentTextBox)
						currentTextBox= UpdateEditorTextBoxes(currentPanel->texts);
					if(!currentTextBox)
					{
						if(g_list_length(currentPanel->buttons) != 0)
							UpdateEditorButtons(currentPanel->buttons);
					}else
					{
						SDL_StartTextInput();
						SDL_SetTextInputRect(&currentTextBox->rect);
						if(GetKeyData()->BackSpace == 1)
							{
								if(mainEvent->key.keysym.sym == SDLK_BACKSPACE && strlen(currentTextBox->value) > 0)
								{
									currentTextBox->value[strlen(currentTextBox->value)-1] = '\0';
									GetKeyData()->BackSpace = 2;
								}
							}
						while(SDL_PollEvent(mainEvent) != 0)
							{
							if(mainEvent->type == SDL_TEXTINPUT)
							{
								strcat(currentTextBox->value,mainEvent->text.text);
							}
						}

					}
				}
							
	}
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
void DecrementFrameNumber(Button* button)
{
	if(workSpace->tileSelector->frameNum >0)
		workSpace->tileSelector->frameNum-=1;
	/*
	x = atoi((char*)button->data);
	x-=1;
	sprintf((char*)button->data,"%d",x);
	*/
}
void IncrementFrameNumber(Button* button)
{
	workSpace->tileSelector->frameNum+=1;
}
void ChangeSpriteSheet(Button* button)
{
	int i;
	GList* elem;
	Sprite *SpriteList = GetSpriteList();
	Label * label = (Label*)button->data;
	if(workSpace->tileSelector != NULL)
	{
		if(strcmp(button->name,"ButtonLeft") == 0)
		{
			elem = g_list_nth(workSpace->tileSelector->SpriteList,workSpace->tileSelector->SpriteSheetListIndex-1);
			if (elem != NULL){
				workSpace->tileSelector->SpriteSheet = (Sprite*)elem->data;
				strcpy(label->name,workSpace->tileSelector->SpriteSheet->filename);
				workSpace->tileSelector->SpriteSheetListIndex--;
				workSpace->tileSelector->frameNum = 0;
			}
		}else if(strcmp(button->name,"ButtonRight") == 0)
		{
			elem = g_list_nth(workSpace->tileSelector->SpriteList,workSpace->tileSelector->SpriteSheetListIndex+1);
			if(elem != NULL)
			{
				workSpace->tileSelector->SpriteSheet = (Sprite*)elem->data;	
				strcpy(label->name,workSpace->tileSelector->SpriteSheet->filename);
				workSpace->tileSelector->SpriteSheetListIndex++;
				workSpace->tileSelector->frameNum = 0;
			}
		}
	}
}

void LoadSpriteSheet(Button* button)
{

}
void LoadEditorMapPanel(Button* button)
{
	Editor_Panel* panel;
	GList *elem;

	TextBox* mapName;
	Button* newMap,*loadMap;
	int i;
	for(i = 0;i < g_list_length(MainEditorPanels);++i)
	{
		elem = g_list_nth(MainEditorPanels,i);
		panel = (Editor_Panel*)elem->data;
		if(strcmp(panel->name,"MapLoadPanel") == 0)
		{
			FreeEditorPanel(panel);
			MainEditorPanels = g_list_remove(MainEditorPanels,panel);
			return;
		}
	}

	panel = CreateEditorPanel(CreateSDL_Rect(100,100,400,130));
	mapName = CreateEditorTextBox(CreateSDL_Rect(130,130,100,100));
	newMap = CreateEditorButton("images/ButtonNew.png",CreateSDL_Rect(400,200,30,16));
	newMap->function = &LoadEditorMapNew;
	panel->buttons = g_list_append(panel->buttons,newMap);


	strcpy(panel->name ,"MapLoadPanel");
	panel->texts = g_list_append(panel->texts,mapName);
	MainEditorPanels = g_list_append(MainEditorPanels,panel);
}

void LoadEditorMap(Button* button)
{

}
void LoadEditorMapNew(Button* button)
{


}
void UpdateMousePosition()
{
	SDL_GetMouseState(&MousePosition.x,&MousePosition.y);
}
SDL_Rect* GetMousePosition()
{
	return &MousePosition;
}
void FreeButton(Button* button)
{
	FreeSprite(button->sprite);
	free(button);
}
void FreeLabel(Label* label)
{
	free(label);
}

void FreeTextBox(TextBox* text)
{
	free(text);
}
void FreeEditorPanel(Editor_Panel *panel)
{
	free(panel);
	if(panel->buttons == NULL)
		g_list_free(panel->buttons);
	if(panel->labels == NULL)
		g_list_free(panel->labels);
	if(panel->panels == NULL)
		g_list_free(panel->panels);
	if(panel->texts == NULL)
		g_list_free(panel->texts);
}
void FreeWorkSpace()
{


}
void FreeEveryThing()
{
	GList *elem,*elem2,*elem3;
	Editor_Panel* ref,*ref2;
	int n,m,k;
		if(g_list_length(MainEditorPanels) != 0){
		for(n = 0; n < g_list_length(MainEditorPanels);++n)
		{
			elem = g_list_nth(MainEditorPanels,n);
			ref = (Editor_Panel*)elem->data;
			if(g_list_length(ref->panels) != 0){
				for(m = 0; m < g_list_length(ref->panels);++m)
				{
					elem2 = g_list_nth(ref->panels,m);
					ref2 = (Editor_Panel*)elem2->data;
					//Free Buttons
					if(g_list_length(ref2->buttons) != 0){
						for(k= 0; k < g_list_length(ref2->buttons);++k)
						{
							elem3 = g_list_nth(ref2->buttons,k);
							FreeButton((Button*)elem3->data);
						}
					}
					if(g_list_length(ref2->labels) != 0){
						for(k = 0; k < g_list_length(ref2->labels);++k)
						{
							elem3 = g_list_nth(ref2->labels,k);
							FreeLabel((Label*)elem3->data);
						}
					}
					if(g_list_length(ref2->texts) != 0){
						for(k = 0; k < g_list_length(ref2->texts);++k)
						{
							elem3 = g_list_nth(ref2->texts,k);
							FreeTextBox((TextBox*)elem3->data);
						}
					}
				g_list_free(ref2->panels);
				FreeEditorPanel(ref2);
			}

		}
		g_list_free(ref->panels);
		FreeEditorPanel(ref);
	}
	}


}

