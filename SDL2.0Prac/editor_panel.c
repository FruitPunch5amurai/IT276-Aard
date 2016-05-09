
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <glib.h>
#include "level.h"
#include "vectors.h"
#include "graphics.h"
#include "editor_createroom.h"
#include "editor_panel.h"
extern Game* game;
extern SDL_Event *mainEvent;
extern Workspace* workSpace;
extern Map *map;
GList* MainEditorPanels = NULL;
Editor_Panel* currentPanel = NULL;
TextBox* currentTextBox = NULL;
SDL_Rect MousePosition;
int ButtonDown = 0;
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
*@brief Creates a Panel
*@param SDL_Rect which is the dimensions and location of the panel
*@return a pointer to the Panel
*/
Editor_Panel* CreateEditorPanel(SDL_Rect rect)
{
	Editor_Panel* p;
	p = (Editor_Panel*)malloc(sizeof(Editor_Panel));
	memset(p,0,sizeof(Editor_Panel));
	p->sprite = NULL;
	p->visible = 1;
	p->panels = NULL;
	p->texts = NULL;
	p->buttons = NULL;
	p->labels = NULL;
	p->rect = rect;
	p->focus = 0;
	return p;
}
/*
*@brief Creates a Label
*@param SDL_Rect which is the dimensions and location of the panel, Char* which is its text
*@return a pointer to the Label
*/
Label* CreateEditorLabel(char* text,SDL_Rect rect)
{
	Label* l;
	l = (Label*)malloc(sizeof(Label));
	memset(l,0,sizeof(Label));
	if(text != NULL)
		strncpy(l->text,text,255);
	l->rect = rect;
	l->sprite = NULL;
	return l;
}
/*
*@brief Creates a Button
*@param SDL_Rect which is the dimensions and location of the panel, char* which is its text
*@return a pointer to the Button
*/
Button* CreateEditorButton(char* text,SDL_Rect rect)
{
	Button *b;
	b = (Button*)malloc(sizeof(Button));
	memset(b,0,sizeof(Button));
	b->clicked = 0;
	b->function = NULL;
	b->rect = rect;
	b->listen = NULL;
	b->inside = 0;
	if (text != NULL)
		b->sprite = LoadSprite(text,rect.w,rect.h);
	else 
		b->sprite = NULL;
	b->soundEffect = Mix_LoadWAV("sound/menu_Click.wav");
	if(b->soundEffect == NULL)
	{
		printf("%s",Mix_GetError());
		exit(1);
	}
	return b;
}
/*
*@brief Creates a Textbox
*@param SDL_Rect which is the dimensions and location of the panel
*@return a pointer to the Textbox
*/
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
/*
*@brief Updates a panels buttons, checks to see if it has been pressed, if so call its function
*@param The GList of buttons of a panel
*/
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
					Mix_PlayChannel (-1,ref->soundEffect, 1);
				}
				else
					inside = false;
			
			}if(inside == true){
				//printf("Mouse inside a button..");
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
/*
*@brief Updates a TextBox
*@param The GList of textboxes of a panel
*@return returns the textbox that is now the focus
*/
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
				//printf("Mouse over a TextBox");
				if(mainEvent->type == SDL_MOUSEBUTTONDOWN)
								//printf("Clicked a textbox");
				memset(ref->value,0,sizeof(char));
				ref->focus = 1;
				currentTextBox = ref;
				return ref;
				}

			}
		}
	return NULL;

	}
/*
*@brief Updates a tile selector
*/
void UpdateTileSelector()
{
	if(workSpace->tileSelector->visible == 1)
		UpdateEditorButtons(workSpace->tileSelector->buttons);
	 if(GetKeyData()->MouseScrollWheelUp == 1)
		{
				workSpace->tileSelector->frameNum +=1;
				sprintf((char*)workSpace->tileSelector->label->text,"%d",workSpace->tileSelector->frameNum);
				GetKeyData()->MouseScrollWheelUp = 0;
	 }else if(GetKeyData()->MouseScrollWheelDown == 1)
			{
				workSpace->tileSelector->frameNum -=1;
				sprintf((char*)workSpace->tileSelector->label->text,"%d",workSpace->tileSelector->frameNum);
				GetKeyData()->MouseScrollWheelDown = 0;
			}
}
/*
*@brief Updates the G_List of panels. This only supports a two tier panel System.
*@param The GList of panels to update
*/
void UpdateEditorPanel(GList* panels)
{
	GList *elem,*elem2;
	Editor_Panel* ref,*ref2;
	TextBox* textBox;
	int n,m;
	UpdateTileSelector();
	if(currentPanel != NULL && currentPanel->visible == 1)
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
		/*
		//Recursive Panel Check
		while(currentPanel->panels != NULL)
		{
			elem = currentPanel->panels;
			for(m = 0; m < g_list_length(elem);++m)
			{
				elem2 = g_list_nth(elem,m);
				ref = (Editor_Panel*)elem2->data;
				currentPanel = ref;
				if(ref->panels == NULL)
					continue;
				else
					break;
			}
		}*/
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
/*
*@brief Updates the Workspace only if currentPanel is NULL.
*/
void UpdateWorkspace()
{
	int x,y,i,n,count;
	SDL_Rect roomOutline;
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
			if(GetKeyData()->MouseButtonDown == 1 && workSpace->mode == Add){
				if(workSpace->activeLayer == 1){
					ChangeTile(workSpace->map->data,
					(GetMousePosition()->x + workSpace->areaToDraw.x)/workSpace->map->tileW,
					(GetMousePosition()->y + workSpace->areaToDraw.y)/workSpace->map->tileH
					,workSpace->tileSelector->frameNum);
				}
				if(workSpace->activeLayer == 2)
				{
					ChangeTile(workSpace->map->data2,
					(GetMousePosition()->x + workSpace->areaToDraw.x)/workSpace->map->tileW,
					(GetMousePosition()->y + workSpace->areaToDraw.y)/workSpace->map->tileH
					,workSpace->tileSelector->frameNum);
				}
				if(workSpace->activeLayer == 3)
				{
					ChangeTile(workSpace->map->data3,
					(GetMousePosition()->x + workSpace->areaToDraw.x)/workSpace->map->tileW,
					(GetMousePosition()->y + workSpace->areaToDraw.y)/workSpace->map->tileH
					,workSpace->tileSelector->frameNum);
					if(!IsTileSolid(map,workSpace->tileSelector->frameNum))
					{
						workSpace->map->solidTiles[workSpace->map->numSolidTiles] = workSpace->tileSelector->frameNum;
						workSpace->map->numSolidTiles+=1;
					}
				}

			}else if(GetKeyData()->MouseButtonDown == 1 && workSpace->mode == Remove)
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
				{
					count = 0;
					for(i = 0; i < workSpace->map->w* workSpace->map->h-1;++i)
					{
						if(workSpace->map->data3[i] == GetTile(workSpace->map->data3,(GetMousePosition()->x + workSpace->areaToDraw.x)/workSpace->map->tileW,
						(GetMousePosition()->y + workSpace->areaToDraw.y)/workSpace->map->tileH))
						{
							count +=1;
							continue;
						}
					}
						if(count == 1)
						{
							workSpace->map->numSolidTiles-=1;
							n = workSpace->map->numSolidTiles;
							for(x = 0;x < 60;++x)
							{
								frame = workSpace->map->solidTiles[n];
								if(frame == workSpace->tileSelector->frameNum)
								{
									for(x = n;x < 60;++x)
									{
										workSpace->map->solidTiles[x] = workSpace->map->solidTiles[x+1];
									}
									break;
								}
								n--;
						}
					}
						ChangeTile(workSpace->map->data3,
					(GetMousePosition()->x + workSpace->areaToDraw.x)/workSpace->map->tileW,
					(GetMousePosition()->y + workSpace->areaToDraw.y)/workSpace->map->tileH
					,0);
				}
			}
		}
		//Get currentRoom
		for(i = 0;i <= workSpace->map->numberOfRooms;++i)
		{
			x = GetMousePosition()->x + workSpace->areaToDraw.x;
			y = GetMousePosition()->y + workSpace->areaToDraw.y;
			if(x > workSpace->map->rooms[i].boundary.x &&
				x< workSpace->map->rooms[i].boundary.x + workSpace->map->rooms[i].boundary.w &&
				y > workSpace->map->rooms[i].boundary.y &&
				y < workSpace->map->rooms[i].boundary.y + workSpace->map->rooms[i].boundary.h
				)
			{
				workSpace->currentRoom = &workSpace->map->rooms[i];
				sprintf(workSpace->RoomNumber->text,"%d",workSpace->currentRoom->id);
			}

		}
	}
}
/*
*@brief Creates an SDL_Rect
*@param Dimensions and location
*@return SDL_Rect
*/
SDL_Rect CreateSDL_Rect(int x,int y,int w,int h)
{
	SDL_Rect rect;
	rect.y = y;
	rect.x = x;
	rect.w = w;
	rect.h = h;
	return rect;
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
	{
		x-=1;
		workSpace->activeLayer -= 1;
	}
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

void SwitchRoomDirectionLeft(Button* button)
{
	char dir[500];
	strcpy(dir,(char*)button->data);
	if(strcmp(dir,"West") == 0)
		strcpy((char*)button->data,"East");
	if(strcmp(dir,"East") == 0)
		strcpy((char*)button->data,"South");
	if(strcmp(dir,"South") == 0)
		strcpy((char*)button->data,"North");

}
void SwitchRoomDirectionRight(Button* button)
{
	char dir[500];
	strcpy(dir,(char*)button->data);
	if(strcmp(dir,"North") == 0)
		strcpy((char*)button->data,"South");
	if(strcmp(dir,"South") == 0)
		strcpy((char*)button->data,"East");
	if(strcmp(dir,"East") == 0)
		strcpy((char*)button->data,"West");

}
/*
*@brief Removes a Panel and Frees it
*@param The Panel to remove and free
*/
void RemovePanel(Editor_Panel* panel)
{
	GList* elem;
	Editor_Panel* ref;
	MainEditorPanels = g_list_remove(panel->siblingPanels,panel);
	FreeEditorPanel(panel);
	elem = g_list_last(MainEditorPanels);
	ref = (Editor_Panel*)elem->data;
	currentPanel = ref;
}
/*
*@brief Changes the currentSprite Sheet to the next on the SpriteSheetList
*@param The Button which has been called
*/
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
/*
*@brief Loads the panel for the Loading of a Map
*@param The Button which has been called
*/
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
	panel->siblingPanels = MainEditorPanels;
	mapName = CreateEditorTextBox(CreateSDL_Rect(130,130,100,100));
	newMap = CreateEditorButton("images/ButtonNew.png",CreateSDL_Rect(400,200,30,16));
	newMap->function = &LoadEditorMapNew;
	newMap->data = mapName;
	newMap->parentPanel = panel;

	loadMap = CreateEditorButton("images/ButtonLoad.png",CreateSDL_Rect(440,200,30,16));
	loadMap->function = &LoadEditorMap;
	loadMap->data = mapName;
	loadMap->parentPanel = panel;

	panel->buttons = g_list_append(panel->buttons,loadMap);
	panel->buttons = g_list_append(panel->buttons,newMap);
	strcpy(panel->name ,"MapLoadPanel");
	panel->texts = g_list_append(panel->texts,mapName);
	MainEditorPanels = g_list_append(MainEditorPanels,panel);
}
void LoadRoomCreatePanel(Button* button)
{
	Editor_Panel* panel;
	GList *elem;
	Button* newRoom;
	Label* labelSizeX,*labelSizeY,*direction,*linksTo,*N,*S,*E,*W;
	TextBox* roomSizeX,*roomSizeY,*NText,*SText,*EText,*WText;
	Button* changeDirectionLeft,*changeDirectionRight;
	int i;
	for(i = 0;i < g_list_length(MainEditorPanels);++i)
	{
		elem = g_list_nth(MainEditorPanels,i);
		panel = (Editor_Panel*)elem->data;
		if(strcmp(panel->name,"RoomCreatePanel") == 0)
		{
			FreeEditorPanel(panel);
			MainEditorPanels = g_list_remove(MainEditorPanels,panel);
			return;
		}
	}

	panel = CreateEditorPanel(CreateSDL_Rect(100,150,400,130));
	panel->siblingPanels = MainEditorPanels;
	labelSizeX =CreateEditorLabel("SizeX",CreateSDL_Rect(panel->rect.x+50,panel->rect.y+10,10,10));
	labelSizeY =CreateEditorLabel("SizeY",CreateSDL_Rect(panel->rect.x+50,panel->rect.y+50,10,10));
	direction = CreateEditorLabel("North",CreateSDL_Rect(panel->rect.x+80,panel->rect.y+100,10,10));
	N = CreateEditorLabel("N",CreateSDL_Rect(panel->rect.x+180,panel->rect.y+30,10,10));
	S = CreateEditorLabel("S",CreateSDL_Rect(panel->rect.x+180,panel->rect.y+60,10,10));
	E = CreateEditorLabel("E",CreateSDL_Rect(panel->rect.x+180,panel->rect.y+90,10,10));
	W = CreateEditorLabel("W",CreateSDL_Rect(panel->rect.x+240,panel->rect.y+30,10,10));
	linksTo = CreateEditorLabel("Links To: ",CreateSDL_Rect(panel->rect.x+180,panel->rect.y+10,10,10));
	
	roomSizeX =CreateEditorTextBox(CreateSDL_Rect(panel->rect.x+140,panel->rect.y+10,10,10));
	roomSizeY = CreateEditorTextBox(CreateSDL_Rect(panel->rect.x+140,panel->rect.y+50,10,10));
	NText = CreateEditorTextBox(CreateSDL_Rect(panel->rect.x+200,panel->rect.y+30,10,10));
	SText = CreateEditorTextBox(CreateSDL_Rect(panel->rect.x+200,panel->rect.y+60,10,10));
	EText = CreateEditorTextBox(CreateSDL_Rect(panel->rect.x+200,panel->rect.y+90,10,10));
	WText = CreateEditorTextBox(CreateSDL_Rect(panel->rect.x+260,panel->rect.y+30,10,10));


	
	changeDirectionLeft = CreateEditorButton("images/ButtonLeft.png",CreateSDL_Rect(panel->rect.x+60,panel->rect.y+105,16,16));
	changeDirectionRight = CreateEditorButton("images/ButtonRight.png",CreateSDL_Rect(panel->rect.x+140,panel->rect.y+105,16,16));
	newRoom = CreateEditorButton("images/ButtonCreate.png",CreateSDL_Rect(panel->rect.x+300,panel->rect.y+100,45,16));
	
		
	//ADD labels
	panel->labels = g_list_append(panel->labels,labelSizeX);
	panel->labels = g_list_append(panel->labels,labelSizeY);
	panel->labels = g_list_append(panel->labels,direction);
	panel->labels = g_list_append(panel->labels,linksTo);
	panel->labels = g_list_append(panel->labels,N);
	panel->labels = g_list_append(panel->labels,S);
	panel->labels = g_list_append(panel->labels,E);
	panel->labels = g_list_append(panel->labels,W);
	labelSizeX->parentPanel = panel;
	labelSizeY->parentPanel = panel;
	direction ->parentPanel = panel;
	linksTo->parentPanel = panel;
	N ->parentPanel = panel;
	S ->parentPanel = panel;
	E ->parentPanel = panel;
	W ->parentPanel = panel;
	labelSizeX->color.r = 200;
	labelSizeY->color.r = 200;
	linksTo->color.r = 200;
	N ->color.r = 200;
	S ->color.r = 200;
	E ->color.r = 200;
	W ->color.r = 200;
	direction ->color.r = 200;
	
	//Add textboxes
	panel->texts = g_list_append(panel->texts,roomSizeX );
	panel->texts = g_list_append(panel->texts,roomSizeY );
	panel->texts = g_list_append(panel->texts,NText);
	panel->texts = g_list_append(panel->texts,SText);
	panel->texts = g_list_append(panel->texts,EText);
	panel->texts = g_list_append(panel->texts,WText);
	strcpy(NText->name,"NorthRoom");
	strcpy(SText->name,"SouthRoom");
	strcpy(EText->name,"EastRoom");
	strcpy(WText->name,"WestRoom");
	strcpy(NText->value,"0");
	strcpy(SText->value,"0");
	strcpy(EText->value,"0");
	strcpy(WText->value,"0");
	strcpy(roomSizeX->name,"RoomSizeX");
	strcpy(roomSizeY->name,"RoomSizeY");
	strcpy(roomSizeX->value,"25");
	strcpy(roomSizeY->value,"19");

	
	//Add Buttons
	panel->buttons = g_list_append(panel->buttons,newRoom);
	panel->buttons = g_list_append(panel->buttons,changeDirectionLeft );
	panel->buttons = g_list_append(panel->buttons,changeDirectionRight);
	newRoom->data = panel;
	newRoom->function = &AddRoomToMap;
	changeDirectionLeft->function = &SwitchRoomDirectionLeft;
	changeDirectionRight->function = &SwitchRoomDirectionRight;
	changeDirectionLeft-> data = direction->text;
	changeDirectionRight->data = direction->text;

	strcpy(panel->name ,"RoomCreatePanel");
	MainEditorPanels = g_list_append(MainEditorPanels,panel);
}
/*
*@brief Loads the map from the data pointed to by the button
*@param The Button which has been called
*/
void LoadEditorMap(Button* button)
{
	char mapName[255];
	TextBox* textBox;
	textBox = (TextBox*)button->data;
	char imageName[255];
	char buf[500];
	int j;
	int NumSolidTiles = 0;
	int mapWidth,mapHeight,tileH,tileW;
	Sprite* sprite;
	
	FILE * file;
	file = fopen(textBox->value,"r");
	if(!file)
	 {
		 printf("Map Does Not Exist\n");
		 return;
	 }else
	 {	
		printf("Map Successfully Loaded\n");
	 }
	while(strcmp(buf,"#Portal") != 0)
	{
		fscanf(file,"%s",buf);
		if(strcmp(buf,"#MapWidth") == 0){
			fscanf(file,"%d",&j);
			mapWidth = j;	
			}
		else if(strcmp(buf,"#MapHeight") == 0)
		{
			fscanf(file,"%d",&j);
			mapHeight = j;
		}
		else if(strcmp(buf,"#TileWidth") == 0)
		{
			fscanf(file,"%d",&j);
			tileW = j;
		}
		else if(strcmp(buf,"#TileHeight") == 0)
		{
			fscanf(file,"%d",&j);
			tileH = j;
			fscanf(file,"%s",buf);
			fscanf(file,"%d",&j);
			 if(map != NULL)
			{
				FreeMap();
			}
			workSpace->map = CreateMap(tileW,tileH,mapWidth,mapHeight,j);
			map = workSpace->map;
			workSpace->map->hasSpecialLayer = 0;
			
			strncpy(workSpace->map->name,textBox->value,128);
			workSpace->map->solidLayer = (int*)malloc(sizeof(int) * workSpace->map->w * workSpace->map->h);
			workSpace->map->data = (int*)malloc(sizeof(int) * workSpace->map->w * workSpace->map->h);
			workSpace->map->data2 = (int*)malloc(sizeof(int) * workSpace->map->w * workSpace->map->h);
			workSpace->map->data3 = (int*)malloc(sizeof(int) * workSpace->map->w * workSpace->map->h);
			
	
			memset(workSpace->map->solidLayer,0,sizeof(workSpace->map->solidLayer));
			memset(workSpace->map->data,0,sizeof(workSpace->map->data));
			memset(workSpace->map->data2,0,sizeof(workSpace->map->data2));
			memset(workSpace->map->data3,0,sizeof(workSpace->map->data3));
		
		}
		else if(strcmp(buf,"#SpriteSheet") == 0)
		{
			fscanf(file,"%s" ,imageName);
			sprite = LoadSprite(imageName,
				workSpace->map->tileW,
				workSpace->map->tileH);
			map->tiles = sprite;
			workSpace->tileSelector->SpriteSheet = sprite;
			workSpace->tileSelector->SpriteList = g_list_append(workSpace->tileSelector->SpriteList,sprite);
		}
		else if(strcmp(buf,"#LevelMusic") == 0)
		{
			fscanf(file,"%s" ,imageName);
			strcpy(map->musicFileName,imageName);
		}
		else if(strcmp(buf,"#NumSolidTiles") == 0)
		{
			fscanf(file,"%d",&j);
			NumSolidTiles = j;
			workSpace->map->numSolidTiles = j;
		}
		else if(strcmp(buf,"#Layer1") == 0)
		{
		LoadLayer(workSpace->map->data,file);
		}
		else if(strcmp(buf,"#Layer2") == 0)
		{
		LoadLayer(workSpace->map->data2,file);
		}
		else if(strcmp(buf,"#Layer3") == 0)
		{
		LoadLayer(workSpace->map->data3,file);
		}
		else if(strcmp(buf,"#SpecialLayer") == 0)
		{
			workSpace->map->specialLayer = LoadSprite("images/Shroud.png",800,600);
			workSpace->map->hasSpecialLayer = 1;
		}
		else if(strcmp(buf,"#SolidTiles") == 0)
		{
		LoadSolidTiles(workSpace->map->solidTiles,file,NumSolidTiles);
		}	
		else if(strcmp(buf,"#MapEntities") == 0)
		{
			SetUpMap(workSpace->map,file);
			break;
		}
	}
	ConnectRooms(workSpace->map);
	workSpace->areaToDraw.x = 0;
	workSpace->areaToDraw.y = 0;
	workSpace->areaToDraw.w = 25 * tileW;
	workSpace->areaToDraw.h = 19 * tileH;
	workSpace->buffer = SDL_CreateTexture(GetRenderer(),SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,map->w * map->tileW,map->h * map->tileH);
	sprite = NULL;
	RemovePanel(button->parentPanel);
}
/*
*@brief Creates a new Map with the data pointed to by the button
*@param The Button which has been called
*/
void LoadEditorMapNew(Button* button)
{
	workSpace->map = CreateMap(32,32,25,19,1);

}
/*
*@brief Hides all the panels so you cant interact with them, also will reveal panels if clicked again
*/
void HidePanels()
{
	GList *elem,*elem2;
	Editor_Panel* ref,*ref2;
	int n,m;
 	if(g_list_length(MainEditorPanels) != 0){
		for(n = g_list_length(MainEditorPanels)-1; n >=0;--n)
		{
			elem = g_list_nth(MainEditorPanels,n);
			ref = (Editor_Panel*)elem->data;
			if(g_list_length(ref->panels) != 0){
				for(m = 0; m < g_list_length(ref->panels);++m)
				{
					elem2 = g_list_nth(ref->panels,m);
					ref2 = (Editor_Panel*)elem2->data;
					ref2->visible = ref2->visible == 1?0:1;
				}
			
			}
		ref->visible = ref->visible == 1?0:1;
		}
	}
	workSpace->tileSelector->visible = workSpace->tileSelector->visible == 1?0:1;
	currentPanel = NULL;
}

void SwitchMode()
{
	workSpace->mode = workSpace->mode == Add?Remove:Add;
}
/*
*@brief Updates the MousesPosition
*/
void UpdateMousePosition()
{
	SDL_GetMouseState(&MousePosition.x,&MousePosition.y);
}
/*
*@brief Gets the mouses current Position
*@return SDL_Rect representing the mouses Position
*/
SDL_Rect* GetMousePosition()
{
	return &MousePosition;
}
/*
*@brief Frees a buttion
*@param The Button to be freed
*/
void FreeButton(Button* button)
{
	FreeSprite(button->sprite);
	free(button);
}
/*
*@brief Frees a label
*@param The label to be freed
*/
void FreeLabel(Label* label)
{
	
	if(label->sprite != NULL)
		FreeSprite(label->sprite);
	free(label);
}
/*
*@brief Frees a Textbox
*@param The textbox to be freed
*/
void FreeTextBox(TextBox* text)
{
	free(text);
}
/*
*@brief Frees a panel
*@param The panel to be freed
*/
void FreeEditorPanel(Editor_Panel *panel)
{
	
	if(panel->sprite !=NULL)
		FreeSprite(panel->sprite);
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
/*
*@brief Frees the workspace
*/
void FreeWorkSpace()
{


}
/*
*@brief Frees all the panels,buttons,labels,textboxes, and workspace in the editor
*/
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
							ref2->buttons = g_list_remove(ref2->buttons,elem3);
							FreeButton((Button*)elem3->data);
						}
					}
					if(g_list_length(ref2->labels) != 0){
						for(k = 0; k < g_list_length(ref2->labels);++k)
						{
							elem3 = g_list_nth(ref2->labels,k);
							ref2->labels = g_list_remove(ref2->labels,elem3);
							FreeLabel((Label*)elem3->data);
						}
					}
					if(g_list_length(ref2->texts) != 0){
						for(k = 0; k < g_list_length(ref2->texts);++k)
						{
							elem3 = g_list_nth(ref2->texts,k);
							ref2->texts = g_list_remove(ref2->texts,elem3);
							FreeTextBox((TextBox*)elem3->data);
						}
					}
				ref->panels = g_list_remove(ref->panels,ref2);
				g_list_free(ref2->panels);		
				FreeEditorPanel(ref2);
			}

		}
		MainEditorPanels = g_list_remove(MainEditorPanels,ref);
		g_list_free(ref->panels);
		FreeEditorPanel(ref);
	}
	}


}

