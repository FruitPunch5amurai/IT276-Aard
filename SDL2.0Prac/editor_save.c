#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <glib.h>
#include "entity.h"
#include "level.h"
#include "vectors.h"
#include "graphics.h"
#include "editor_save.h"
extern Workspace* workSpace;

void SaveMap(Button* button)
{
	GList *elem,*elem2;
	EntityBluePrint *ref,*ref2;
	FILE * file;
	char buf[500];
	char filebuf[500];
	char temp[500];
	int x,y;
	int j;
	int num;
	int bytes = 0;
	file = fopen(workSpace->map->name,"w+");
	if(!file)
	 {
		 printf("Map Does Not Exist\n");
		 return;
	 }else
	 {	
		printf("Map Successfully Loaded\n");
	 }
		//#MapWidth
		fputs("#MapWidth		",file);
		sprintf(buf,"%d",workSpace->map->w);
		fputs(buf,file);
		fputs("\n",file);

		//#MapHeight
		fputs("#MapHeight		",file);
		sprintf(buf,"%d",workSpace->map->h);
		fputs(buf,file);
		fputs("\n",file);
		//#TileWidth
		fputs("#TileWidth		",file);
		sprintf(buf,"%d",workSpace->map->tileW);
		fputs(buf,file);
		fputs("\n",file);
		//#TileHeight
		fputs("#TileHeight		",file);
		sprintf(buf,"%d",workSpace->map->tileH);
		fputs(buf,file);
		fputs("\n",file);
		//#NumOfRooms
		fputs("#NumOfRooms		",file);
		sprintf(buf,"%d",workSpace->map->numberOfRooms);
		fputs(buf,file);
		fputs("\n",file);
		//#NumSolidTiles
		fputs("#NumSolidTiles		",file);
		sprintf(buf,"%d",workSpace->map->numSolidTiles);
		fputs(buf,file);
		fputs("\n",file);
		//#SpriteSheet
		fputs("#SpriteSheet		",file);
		sprintf(buf,"%s",workSpace->map->tiles->filename);
		fputs(buf,file);
		fputs("\n",file);
		//#Layer1
		fputs("\n",file);
		fputs("#Layer1\n",file);
		for(y = 0;y < workSpace->map->h;++y)
			{
				for(x = 0;x < workSpace->map->w;++x)
				{
					if(workSpace->map->data[y*workSpace->map->w + x] < 10)
						fputs("0",file);
					sprintf(buf,"%d", workSpace->map->data[y*workSpace->map->w + x]);
					fputs(buf,file);
					fputs(" ",file);
				}
					fputs("\n",file);
			}
		//#Layer2
		fputs("\n",file);
		fputs("#Layer2\n",file);
		for(y = 0;y < workSpace->map->h;++y)
			{
				for(x = 0;x < workSpace->map->w;++x)
				{
					if(workSpace->map->data2[y*workSpace->map->w + x] < 10)
						fputs("0",file);
					sprintf(buf,"%d", workSpace->map->data2[y*workSpace->map->w + x]);
					fputs(buf,file);
					fputs(" ",file);
				}
					fputs("\n",file);
			}
		//#Layer3
		fputs("\n",file);
		fputs("#Layer3\n",file);
		for(y = 0;y < workSpace->map->h;++y)
			{
				for(x = 0;x < workSpace->map->w;++x)
				{
					if(workSpace->map->data3[y*workSpace->map->w + x] < 10)
						fputs("0",file);
					sprintf(buf,"%d", workSpace->map->data3[y*workSpace->map->w + x]);
					fputs(buf,file);
					fputs(" ",file);
				}
					fputs("\n",file);
			}
		//#SolidTiles
		fputs("\n",file);
		fputs("#SolidTiles\n",file);
		for(j = 0;j <workSpace->map->numSolidTiles;++j)
		{
			sprintf(buf,"%d",workSpace->map->solidTiles[j]);
			fputs(buf,file);
			fputs(" ",file);
		}
		//#MapEntities
		//#RoomIds
		fputs("\n",file);
		fputs("#MapEntities\n",file);
		fputs("#RoomIds\n\n",file);

		for(j = 0;j < workSpace->map->numberOfRooms;++j)
		{
			sprintf(buf,"RoomId: %d \nLinksTo: ",
				workSpace->map->rooms[j].id);
			if(workSpace->map->rooms[j].north != NULL){
				sprintf(temp,"%d , ",workSpace->map->rooms[j].north->id);
				strcat(buf,temp);
			}else
			{
				strcat(buf,"0 , ");
			}
			if(workSpace->map->rooms[j].south != NULL){
				sprintf(temp,"%d , ",workSpace->map->rooms[j].south->id);
				strcat(buf,temp);
			}else
			{
				strcat(buf,"0 , ");
			}
			if(workSpace->map->rooms[j].east != NULL){
				sprintf(temp,"%d , ",workSpace->map->rooms[j].east->id);
				strcat(buf,temp);
			}else
			{
				strcat(buf,"0 , ");
			}
			if(workSpace->map->rooms[j].west != NULL){
				sprintf(temp,"%d",workSpace->map->rooms[j].west->id);
				strcat(buf,temp);
			}
			else
			{
				strcat(buf,"0");
			}
			strcat(buf,"\n");
			fputs(buf,file);
			sprintf(buf,
				"Boundary: %d , %d \n"
				"Location: %d , %d \n",
				workSpace->map->rooms[j].boundary.w,
				workSpace->map->rooms[j].boundary.h,
				workSpace->map->rooms[j].boundary.x,
				workSpace->map->rooms[j].boundary.y);
			fputs (buf,file);
			if(g_list_length(workSpace->map->rooms[j].Entities) > 0)
				fputs("Entities: \n", file);
			for(x = 0; x < g_list_length(workSpace->map->rooms[j].Entities);++x)
			{
				elem = g_list_nth(workSpace->map->rooms[j].Entities,x);
				ref = (EntityBluePrint*)elem->data;
				if(ref->entType == Enemy)
				{
					sprintf(buf,"Type: %s  Count: %d  ",
						Enemy_String[ref->enemyType],ref->count);
					fputs(buf,file);
					sprintf(buf,"Location: ");
					for(y = 0; y < g_list_length(workSpace->map->rooms[j].Entities);++y)
					{
						elem2 = g_list_nth(workSpace->map->rooms[j].Entities,y);
						ref2 = (EntityBluePrint*)elem2->data;
						if(ref2->entType == Enemy)
						{
							if(ref2->enemyType == ref->enemyType)
							{
								sprintf(temp," %.1f , %.1f ",ref2->location.x/32,ref2->location.y/32);
								strcat(buf,temp);			

							}
						}
					}
					fputs (buf,file);
					fputs("\n",file);
				}
				else if(ref->entType == Dungeon)
				{
					sprintf(buf,"Dungeon To: %s Size: %d x %d Location: %.1f , %.1f"
						"PlayerSpawn: %.1f  , %.1f",ref->filename,ref->sizex,ref->sizey,
						ref->location.x,ref->location.y,ref->playerSpawn.x,ref->playerSpawn.y);
				}

			}
			fputs ("#end\n\n",file);

		}

	 fclose(file);
}