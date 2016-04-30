
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
	ws->activeLayer = 0;
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
	strncpy(l->text,text,255);
	l->rect = rect;
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
	b->sprite = LoadSprite(text,32,16);							/*< for now */
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
/*
*@brief Updates a tile selector
*/
void UpdateTileSelector()
{
	UpdateEditorButtons(workSpace->tileSelector->buttons);
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
	if(currentPanel != NULL)
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
	if(workSpace->tileSelector->frameNum >0)
		workSpace->tileSelector->frameNum-=1;
	/*
	x = atoi((char*)button->data);
	x-=1;
	sprintf((char*)button->data,"%d",x);
	*/
}
/*
*@brief Increases the frame number for the TileSelector
*@param The Button which has been called
*/
void IncrementFrameNumber(Button* button)
{
	workSpace->tileSelector->frameNum+=1;
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
	while(strcmp(buf,"#SolidTiles") != 0)
	{
		fscanf(file,"%s",buf);
		if(strcmp(buf,"#MapWidth") == 0){
			fscanf(file,"%d",&j);
			mapWidth = j;	
			}
		if(strcmp(buf,"#MapHeight") == 0)
		{
			fscanf(file,"%d",&j);
			mapHeight = j;
		}
		if(strcmp(buf,"#TileWidth") == 0)
		{
			fscanf(file,"%d",&j);
			tileW = j;
		}
		if(strcmp(buf,"#TileHeight") == 0)
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
		if(strcmp(buf,"#SpriteSheet") == 0)
		{
			fscanf(file,"%s" ,imageName);
			map->tiles = LoadSprite(imageName,tileW,tileH);
		}
		if(strcmp(buf,"#NumSolidTiles") == 0)
		{
			fscanf(file,"%d",&j);
			NumSolidTiles = j;
			workSpace->map->numSolidTiles = j;
		}
		if(strcmp(buf,"#Layer1") == 0)
		{
		LoadLayer(workSpace->map->data,file);
		}
		if(strcmp(buf,"#Layer2") == 0)
		{
		LoadLayer(workSpace->map->data2,file);
		}
		if(strcmp(buf,"#Layer3") == 0)
		{
		LoadLayer(workSpace->map->data3,file);
		}
		if(strcmp(buf,"#SpecialLayer") == 0)
		{
			workSpace->map->specialLayer = LoadSprite("images/Shroud.png",800,600);
			workSpace->map->hasSpecialLayer = 1;
		}
		if(strcmp(buf,"#SolidTiles") == 0)
		{
		LoadSolidTiles(workSpace->map->solidTiles,file,NumSolidTiles);
		}	
	}
	workSpace->buffer = SDL_CreateTexture(GetRenderer(),SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,map->w * map->tileW,map->h * map->tileH);
	RemovePanel(button->parentPanel);
}
/*
*@brief Creates a new Map with the data pointed to by the button
*@param The Button which has been called
*/
void LoadEditorMapNew(Button* button)
{


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

