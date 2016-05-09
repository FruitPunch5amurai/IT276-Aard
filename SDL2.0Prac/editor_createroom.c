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
	int x,y,n,temp,temp2,temp3,temp4,count;
	int boundrySizeX,boundrySizeY;
	GList* elem;
	TextBox* TextBoxRef;
	Label* LabelRef;
	Room* newRoom;
	Editor_Panel* panel;
	SDL_Rect boundary;
	int linksTo[4] = {0,0,0,0};

	workSpace->map->numberOfRooms += 1;
	panel = (Editor_Panel*)button->data;
	TextBoxRef = (TextBox*)g_list_nth_data(panel->texts,0);
	boundary.w = atoi(TextBoxRef->value);
	TextBoxRef = (TextBox*)g_list_nth_data(panel->texts,1);
	boundary.h = atoi(TextBoxRef->value);
	LabelRef = (Label*)g_list_nth_data(panel->labels,2);
	TextBoxRef = (TextBox*)g_list_nth_data(panel->texts,2);
	linksTo[0] = atoi(TextBoxRef->value);
	TextBoxRef = (TextBox*)g_list_nth_data(panel->texts,3);
	linksTo[1] = atoi(TextBoxRef->value);
	TextBoxRef = (TextBox*)g_list_nth_data(panel->texts,4);
	linksTo[2] = atoi(TextBoxRef->value);
	TextBoxRef = (TextBox*)g_list_nth_data(panel->texts,5);
	linksTo[3] = atoi(TextBoxRef->value);

	//North
	if(strcmp(LabelRef->text,"North") == 0)
	{
		if(workSpace->currentRoom->boundary.y == 0)
		{
			
			for(i =0;i < workSpace->map->numberOfRooms-1;++i)
			{
				workSpace->map->rooms[i].boundary.y += 600;
			}
			temp = (boundary.h/20)+1;
			workSpace->map->h += 19 * temp;
			boundary.h = boundary.h * workSpace->map->tileH;
			boundary.w = boundary.w * workSpace->map->tileW;
			workSpace->map->data = (int *)realloc(workSpace->map->data,sizeof(int)*workSpace->map->w*workSpace->map->h);
			workSpace->map->data2 = (int *)realloc(workSpace->map->data2,sizeof(int)*workSpace->map->w*workSpace->map->h);
			workSpace->map->data3 = (int *)realloc(workSpace->map->data3,sizeof(int)*workSpace->map->w*workSpace->map->h);
			workSpace->map->solidLayer = (int *)realloc(workSpace->map->solidLayer ,sizeof(int)*workSpace->map->w*workSpace->map->h);
			n = workSpace->map->w* workSpace->map->h-1;
			for( x = n; x >workSpace-> map->w * (workSpace->map->h - (19*temp))-1; --x)
			{
				workSpace->map->data[x] = 0;
				workSpace->map->data2[x] = 0;
				workSpace->map->data3[x] = 0;
				workSpace->map->solidLayer[x] = 0;

			}
			for(x = workSpace->map->w*(workSpace->map->h-(19*temp))-1; x >= 0;--x)
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
		//connect rooms;
		workSpace->currentRoom->roomIDs[0] = workSpace->map->numberOfRooms-1;
		linksTo[1] = workSpace->currentRoom->id;
		boundary.x = workSpace->currentRoom->boundary.x;
		boundary.y = workSpace->currentRoom->boundary.y - 600 * temp;
		newRoom = CreateRoom(workSpace->map->numberOfRooms-1,&boundary,linksTo);
		for(i = 0;i<4;++i)
		{
			if(linksTo[i] != 0)
			{
				if(i == 0)
					FindRoomWithID(linksTo[i])->roomIDs[1]= workSpace->map->numberOfRooms-1;
				else if(i == 1)
					FindRoomWithID(linksTo[i])->roomIDs[0] = workSpace->map->numberOfRooms-1;
				else if(i == 2)
					FindRoomWithID(linksTo[i])->roomIDs[4] = workSpace->map->numberOfRooms-1;
				else if (i == 3)
					FindRoomWithID(linksTo[i])->roomIDs[3] = workSpace->map->numberOfRooms-1;
			}
		}
		workSpace->currentRoom->north = newRoom;
		newRoom->south = workSpace->currentRoom;
		ConnectRooms(workSpace->map);

		
	}else if(strcmp(LabelRef->text,"East") == 0)
	{
		if(workSpace->currentRoom->boundary.x == 0)
		{
			
			for(i =0;i<workSpace->map->numberOfRooms-1;++i)
			{
				workSpace->map->rooms[i].boundary.x += 800;
			}
			boundrySizeX = (boundary.w/26)+1;
			workSpace->map->w += 25 * (boundrySizeX);
			boundary.h = boundary.h * workSpace->map->tileH;
			boundary.w = boundary.w * workSpace->map->tileW;
			workSpace->map->data = (int *)realloc(workSpace->map->data,sizeof(int)*workSpace->map->w*workSpace->map->h);
			workSpace->map->data2 = (int *)realloc(workSpace->map->data2,sizeof(int)*workSpace->map->w*workSpace->map->h);
			workSpace->map->data3 = (int *)realloc(workSpace->map->data3,sizeof(int)*workSpace->map->w*workSpace->map->h);
			workSpace->map->solidLayer = (int *)realloc(workSpace->map->solidLayer ,sizeof(int)*workSpace->map->w*workSpace->map->h);
			n = workSpace->map->w* workSpace->map->h-1;
			for( x = n; x >((workSpace-> map->w-(25*boundrySizeX)) -1) * workSpace->map->h; --x)
			{
				workSpace->map->data[x] = 0;
				workSpace->map->data2[x] = 0;
				workSpace->map->data3[x] = 0;
				workSpace->map->solidLayer[x] = 0;
			}
			count = 0;
			for(x = 0; x <= (workSpace-> map->w) * workSpace->map->h-1;++x)
			{
				temp = workSpace->map->data[n];
				temp2 = workSpace->map->data2[n];
				temp3 = workSpace->map->data3[n]; 
				temp4 = workSpace->map->solidLayer[n];
				for(y = n; y > x;--y)
				{
					workSpace->map->data[y] = workSpace->map->data[y-1];
					workSpace->map->data2[y] = workSpace->map->data2[y-1];
					workSpace->map->data3[y] = workSpace->map->data3[y-1];
					workSpace->map->solidLayer[y] = workSpace->map->solidLayer[y-1];
				}
				workSpace->map->data[x] = temp;
				workSpace->map->data2[x] = temp2;
				workSpace->map->data3[x] = temp3;
				workSpace->map->solidLayer[x] = temp4;
				count+=1;
				if(count == 25*boundrySizeX)
				{
					x += workSpace->map->w -25;
					count = 0;
				}
			}
		}
		//connect rooms;
		
		workSpace->currentRoom->roomIDs[2] = workSpace->map->numberOfRooms-1;
		linksTo[3] = workSpace->currentRoom->id;
		boundary.x = workSpace->currentRoom->boundary.x- 800 * boundrySizeX;
		boundary.y = workSpace->currentRoom->boundary.y;
		newRoom = CreateRoom(workSpace->map->numberOfRooms-1,&boundary,linksTo);
		for(i = 0;i<4;++i)
		{
			if(linksTo[i] != 0)
			{
				if(i == 0)
					FindRoomWithID(linksTo[i])->roomIDs[1]= workSpace->map->numberOfRooms-1;
				else if(i == 1)
					FindRoomWithID(linksTo[i])->roomIDs[0] = workSpace->map->numberOfRooms-1;
				else if(i == 2)
					FindRoomWithID(linksTo[i])->roomIDs[3] = workSpace->map->numberOfRooms-1;
				else if (i == 3)
					FindRoomWithID(linksTo[i])->roomIDs[2] = workSpace->map->numberOfRooms-1;
			}
		}
		workSpace->currentRoom->east = newRoom;
		newRoom->west = workSpace->currentRoom;
		ConnectRooms(workSpace->map);
		
	}else if(strcmp(LabelRef->text,"South") == 0)
	{
			temp = (boundary.h/20)+1;
			workSpace->map->h += 19 * temp;
			boundary.h = boundary.h * workSpace->map->tileH;
			boundary.w = boundary.w * workSpace->map->tileW;
			workSpace->map->data = (int *)realloc(workSpace->map->data,sizeof(int)*workSpace->map->w*workSpace->map->h);
			workSpace->map->data2 = (int *)realloc(workSpace->map->data2,sizeof(int)*workSpace->map->w*workSpace->map->h);
			workSpace->map->data3 = (int *)realloc(workSpace->map->data3,sizeof(int)*workSpace->map->w*workSpace->map->h);
			workSpace->map->solidLayer = (int *)realloc(workSpace->map->solidLayer ,sizeof(int)*workSpace->map->w*workSpace->map->h);
			n = workSpace->map->w* workSpace->map->h-1;
			for( x = n; x >workSpace-> map->w * (workSpace->map->h - (19*temp))-1; --x)
			{
				workSpace->map->data[x] = 0;
				workSpace->map->data2[x] = 0;
				workSpace->map->data3[x] = 0;
				workSpace->map->solidLayer[x] = 0;

			}
		workSpace->currentRoom->roomIDs[1] = workSpace->map->numberOfRooms-1;
		linksTo[0] = workSpace->currentRoom->id;
		boundary.x = workSpace->currentRoom->boundary.x;
		boundary.y = workSpace->currentRoom->boundary.y + 600 *(workSpace->currentRoom->boundary.h/600);
		newRoom = CreateRoom(workSpace->map->numberOfRooms-1,&boundary,linksTo);
		for(i = 0;i<4;++i)
		{
			if(linksTo[i] != 0)
			{
				if(i == 0)
					FindRoomWithID(linksTo[i])->roomIDs[1]= workSpace->map->numberOfRooms-1;
				else if(i == 1)
					FindRoomWithID(linksTo[i])->roomIDs[0] = workSpace->map->numberOfRooms-1;
				else if(i == 2)
					FindRoomWithID(linksTo[i])->roomIDs[3] = workSpace->map->numberOfRooms-1;
				else if (i == 3)
					FindRoomWithID(linksTo[i])->roomIDs[2] = workSpace->map->numberOfRooms-1;
			}
		}
		workSpace->currentRoom->south = newRoom;
		newRoom->north = workSpace->currentRoom;
		ConnectRooms(workSpace->map);

	}else if(strcmp(LabelRef->text,"West") == 0)
	{
		workSpace->currentRoom->roomIDs[3] = workSpace->map->numberOfRooms-1;
		linksTo[2] = workSpace->currentRoom->id;
		boundary.x = workSpace->currentRoom->boundary.x+800;
		boundary.y = workSpace->currentRoom->boundary.y;
		boundary.h = boundary.h * workSpace->map->tileH;
		boundary.w = boundary.w * workSpace->map->tileW;
		newRoom = CreateRoom(workSpace->map->numberOfRooms-1,&boundary,linksTo);
		for(i = 0;i<4;++i)
		{
			if(linksTo[i] != 0)
			{
				if(i == 0)
					FindRoomWithID(linksTo[i])->roomIDs[1]= workSpace->map->numberOfRooms-1;
				else if(i == 1)
					FindRoomWithID(linksTo[i])->roomIDs[0] = workSpace->map->numberOfRooms-1;
				else if(i == 2)
					FindRoomWithID(linksTo[i])->roomIDs[3] = workSpace->map->numberOfRooms-1;
				else if (i == 3)
					FindRoomWithID(linksTo[i])->roomIDs[2] = workSpace->map->numberOfRooms-1;
			}
		}
		workSpace->currentRoom->west = newRoom;
		newRoom->east = workSpace->currentRoom;
		ConnectRooms(workSpace->map);

	}
			SDL_SetRenderTarget(GetRenderer(),workSpace->buffer);
			SDL_RenderClear(GetRenderer());
			SDL_SetRenderTarget(GetRenderer(),NULL);
			SDL_DestroyTexture(workSpace->buffer);
			workSpace->buffer = SDL_CreateTexture(GetRenderer(),SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,workSpace->map->w * workSpace->map->tileW,workSpace->map->h * workSpace->map->tileH);
}