#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "graphics.h"
#include "level.h"
#include "entity.h"
#include "editor_createroom.h"
extern Workspace *workSpace;

void AddRoomToMap(Button* button)
{
	int i;
	int x,y,n,temp;
	GList* elem;
	TextBox* TextBoxRef;
	Label* LabelRef;
	Room* newRoom;
	Editor_Panel* panel;
	SDL_Rect boundary;
	int linksTo[4] = {0,0,0,0};
	workSpace->map->numberOfRooms += 1;
	workSpace->map->rooms = (Room*)malloc(sizeof(Room*)*workSpace->map->numberOfRooms);
	panel = (Editor_Panel*)button->data;
	TextBoxRef = (TextBox*)g_list_nth_data(panel->texts,0);
	boundary.w = atoi(TextBoxRef->value);
	TextBoxRef = (TextBox*)g_list_nth_data(panel->texts,1);
	boundary.h = atoi(TextBoxRef->value);
	LabelRef = (Label*)g_list_nth_data(panel->labels,0);

	//North
	if(strcmp(LabelRef->text,"North"))
	{
		if(workSpace->currentRoom->boundary.y == 0)
		{
			
			for(i =0;i<workSpace->map->numberOfRooms-1;++i)
			{
				workSpace->map->rooms[i].boundary.y += 600;
			}
			workSpace->map->h += 19;
			workSpace->map->data = (int *)realloc(workSpace->map->data,sizeof(int)*workSpace->map->w*workSpace->map->h);
			workSpace->map->data2 = (int *)realloc(workSpace->map->data2,sizeof(int)*workSpace->map->w*workSpace->map->h);
			workSpace->map->data3 = (int *)realloc(workSpace->map->data3,sizeof(int)*workSpace->map->w*workSpace->map->h);
			workSpace->map->solidLayer = (int *)realloc(workSpace->map->solidLayer ,sizeof(int)*workSpace->map->w*workSpace->map->h);
			n = workSpace->map->w* workSpace->map->h-1;
			for( x = n; x >workSpace-> map->w * (workSpace->map->h - 19)-1; --x)
			{
				workSpace->map->data[x] = 0;
				workSpace->map->data2[x] = 0;
				workSpace->map->data3[x] = 0;
				workSpace->map->solidLayer[x] = 0;

			}
			for(x = workSpace->map->w*(workSpace->map->h-19)-1; x >= 0;--x)
			{
				workSpace->map->data[n] =workSpace-> map->data[x];
				workSpace->map->data2[n] = workSpace->map->data2[x];
				workSpace->map->data3[n] = workSpace->map->data3[x];
				workSpace->map->solidLayer[n] = workSpace->map->solidLayer[x];
				workSpace->map->data[x] = 0;
				workSpace->map->data2[x] = 0;
				workSpace->map->data3[x] = 0;
				workSpace->map->solidLayer[x] = 0;
				n--;
			}
		}
		/*
		room->roomIDs[0] = map->numberOfRooms;
		linksTo[1] = room->id;
		boundary.x = room->boundary.x;
		boundary.y = room->boundary.y - 600;
		newRoom = CreateRoom(map->numberOfRooms,&boundary,linksTo);
		room->north = newRoom;
		newRoom->south = room;
		map->rooms[map->numberOfRooms] = *newRoom;
		*/
	}
			SDL_SetRenderTarget(GetRenderer(),workSpace->buffer);
			SDL_RenderClear(GetRenderer());
			SDL_SetRenderTarget(GetRenderer(),NULL);
			SDL_DestroyTexture(workSpace->buffer);
			workSpace->buffer = SDL_CreateTexture(GetRenderer(),SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,workSpace->map->w * workSpace->map->tileW,workSpace->map->h * workSpace->map->tileH);
}