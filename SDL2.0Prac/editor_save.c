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
extern Map* map;
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
	file = fopen(map->name,"w+");
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
		sprintf(buf,"%d",map->w);
		fputs(buf,file);
		fputs("\n",file);

		//#MapHeight
		fputs("#MapHeight		",file);
		sprintf(buf,"%d",map->h);
		fputs(buf,file);
		fputs("\n",file);
		//#TileWidth
		fputs("#TileWidth		",file);
		sprintf(buf,"%d",map->tileW);
		fputs(buf,file);
		fputs("\n",file);
		//#TileHeight
		fputs("#TileHeight		",file);
		sprintf(buf,"%d",map->tileH);
		fputs(buf,file);
		fputs("\n",file);
		//#NumOfRooms
		fputs("#NumOfRooms		",file);
		sprintf(buf,"%d",map->numberOfRooms);
		fputs(buf,file);
		fputs("\n",file);
		//#NumSolidTiles
		fputs("#NumSolidTiles		",file);
		sprintf(buf,"%d",map->numSolidTiles);
		fputs(buf,file);
		fputs("\n",file);
		//#SpriteSheet
		fputs("#SpriteSheet		",file);
		sprintf(buf,"%s",map->tiles->filename);
		fputs(buf,file);
		fputs("\n",file);
		//LevelMusic
		fputs("#LevelMusic	",file);
		sprintf(buf,"%s",map->musicFileName);
		fputs(buf,file);
		fputs("\n",file);
		//#Layer1
		fputs("\n",file);
		fputs("#Layer1\n",file);
		for(y = 0;y < map->h;++y)
			{
				for(x = 0;x < map->w;++x)
				{
					if(map->data[y*map->w + x] < 0)
					{
						fputs("  ",file);
						fputs("00",file);
					}
					else if(map->data[y*map->w + x] < 10)
					{
						fputs("  ",file);
						fputs("0",file);
						sprintf(buf,"%d", map->data[y*map->w + x]);
						fputs(buf,file);
						
					}
					else if(map->data[y*map->w + x] > 99)
					{
						fputs(" ",file);
					sprintf(buf,"%d", map->data[y*map->w + x]);
					fputs(buf,file);
					}
					else
					{
						fputs("  ",file);
						sprintf(buf,"%d", map->data[y*map->w + x]);
						fputs(buf,file);
					}

				}
					fputs("\n",file);
			}
		//#Layer2
		fputs("\n",file);
		fputs("#Layer2\n",file);
		for(y = 0;y < map->h;++y)
			{
				for(x = 0;x < map->w;++x)
				{
					if(map->data2[y*map->w + x] < 0)
					{
						fputs("  ",file);
						fputs("00",file);
						continue;
					}
					else if(map->data2[y*map->w + x] < 10)
					{
						fputs("  ",file);
						fputs("0",file);
						sprintf(buf,"%d", map->data2[y*map->w + x]);
						fputs(buf,file);
						
					}
					else if(map->data2[y*map->w + x] > 99)
					{
						fputs(" ",file);
					sprintf(buf,"%d", map->data2[y*map->w + x]);
					fputs(buf,file);
					}else
					{
						fputs("  ",file);
						sprintf(buf,"%d", map->data2[y*map->w + x]);
						fputs(buf,file);
					}
				}
					fputs("\n",file);
			}
		//#Layer3
		fputs("\n",file);
		fputs("#Layer3\n",file);
		for(y = 0;y < map->h;++y)
			{
				for(x = 0;x < map->w;++x)
				{
					if(map->data3[y*map->w + x] < 0)
					{
						fputs("  ",file);
						fputs("00",file);
						continue;
					}
					else if(map->data3[y*map->w + x] < 10)
					{
						fputs("  ",file);
						fputs("0",file);
						sprintf(buf,"%d", map->data3[y*map->w + x]);
						fputs(buf,file);
						
					}
					else if(map->data3[y*map->w + x] > 99)
					{
						fputs(" ",file);
					sprintf(buf,"%d", map->data3[y*map->w + x]);
					fputs(buf,file);
					}else
					{
						fputs("  ",file);
						sprintf(buf,"%d", map->data3[y*map->w + x]);
						fputs(buf,file);
					}
				}
					fputs("\n",file);
			}
		//#special Layer
		if(map->hasSpecialLayer == 1)
			fputs("#SpecialLayer\n",file);
		//#SolidTiles
		fputs("\n",file);
		fputs("#SolidTiles\n",file);
		for(j = 0;j <map->numSolidTiles;++j)
		{
			sprintf(buf,"%d",map->solidTiles[j]);
			fputs(buf,file);
			fputs(" ",file);
		}
		//#MapEntities
		//#RoomIds
		fputs("\n",file);
		fputs("#MapEntities\n",file);
		fputs("#RoomIds\n\n",file);

		for(j = 0;j < map->numberOfRooms;++j)
		{
			sprintf(buf,"RoomId: %d \nLinksTo: ",
				map->rooms[j].id);
			if(map->rooms[j].roomIDs[0] != 0){
				sprintf(temp,"%d , ",map->rooms[j].north->id);
				strcat(buf,temp);
			}else
			{
				strcat(buf,"0 , ");
			}
			if(map->rooms[j].roomIDs[1] != 0){
				sprintf(temp,"%d , ",map->rooms[j].south->id);
				strcat(buf,temp);
			}else
			{
				strcat(buf,"0 , ");
			}
			if(map->rooms[j].roomIDs[2] != 0){
				sprintf(temp,"%d , ",map->rooms[j].east->id);
				strcat(buf,temp);
			}else
			{
				strcat(buf,"0 , ");
			}
			if(map->rooms[j].roomIDs[3] != 0){
				sprintf(temp,"%d",map->rooms[j].west->id);
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
				map->rooms[j].boundary.w,
				map->rooms[j].boundary.h,
				map->rooms[j].boundary.x,
				map->rooms[j].boundary.y);
			fputs (buf,file);
			if(g_list_length(map->rooms[j].Entities) > 0)
				fputs("Entities: \n", file);
			for(x = 0; x < g_list_length(map->rooms[j].Entities);++x)
			{
				elem = g_list_nth(map->rooms[j].Entities,x);
				ref = (EntityBluePrint*)elem->data;
				if(ref->entType == Enemy)
				{
					sprintf(buf,"Type: %s  Count: %d  ",
						Enemy_String[ref->enemyType],ref->count);
					fputs(buf,file);
					sprintf(buf,"Location: ");
					for(y = 0; y < g_list_length(map->rooms[j].Entities);++y)
					{
						elem2 = g_list_nth(map->rooms[j].Entities,y);
						ref2 = (EntityBluePrint*)elem2->data;
						if(ref2->entType == Enemy)
						{
							if(ref2->enemyType == ref->enemyType)
							{
								sprintf(temp," %.0f , %.0f ",(ref2->location.x - map->rooms[j].boundary.x)/32,(ref2->location.y - map->rooms[j].boundary.y )/32);
								strcat(buf,temp);			

							}
						}
					}
					fputs (buf,file);
					fputs("\n",file);
				}
				else if(ref->entType == Dungeon)
				{
					sprintf(buf,"Type: Dungeon To: %s Size: %d x %d Location: %.0f , %.0f "
						"PlayerSpawn: %.0f  , %.0f \n",ref->filename,ref->sizex,ref->sizey,
						(ref->location.x - map->rooms[j].boundary.x )/32,(ref->location.y - map->rooms[j].boundary.y)/32,ref->playerSpawn.x/32,ref->playerSpawn.y/32);
					fputs(buf,file);
				}else if (ref->entType == Spirit)
				{
					sprintf(buf,"Type: %s  Count: %d  ",
						"Spirit",ref->count);
					fputs(buf,file);
					sprintf(buf,"Location: ");
					for(y = 0; y < g_list_length(map->rooms[j].Entities);++y)
					{
						elem2 = g_list_nth(map->rooms[j].Entities,y);
						ref2 = (EntityBluePrint*)elem2->data;
						if(ref2->entType == Spirit)
						{
							sprintf(temp," %.0f , %.0f ",(ref2->location.x-map->rooms[j].boundary.x)/ref2->sizex,
								(ref2->location.y-map->rooms[j].boundary.y)/ref2->sizey);
								strcat(buf,temp);			
						}
					}
					fputs (buf,file);
					fputs("\n",file);
				}else if(ref->entType == BreakableObject)
				{
					sprintf(buf,"Type: Breakable Frame: %d Filename: %s Count: %d Size %d x %d ",
						ref->frame,ref->filename,ref->count,ref->sizex,ref->sizey);
					fputs (buf,file);
					sprintf(buf,"Location: ");
					for(y = 0; y < g_list_length(map->rooms[j].Entities);++y)
					{
						elem2 = g_list_nth(map->rooms[j].Entities,y);
						ref2 = (EntityBluePrint*)elem2->data;
						if(ref2->entType == BreakableObject)
						{
								sprintf(temp," %.0f , %.0f ",(ref2->location.x -map->rooms[j].boundary.x )/ref2->sizex,(ref2->location.y - map->rooms[j].boundary.y)/ref2->sizey);
								strcat(buf,temp);			
						}
					}
					fputs (buf,file);
					fputs("\n",file);

				}else if(ref->entType == Chest)
				{
					if(ref->ref == NULL)
					{
						sprintf(buf,"Type: Chest Item: NONE Frame: %d Filename: %s Count: %d Size %d x %d ",
					ref->frame,ref->filename,ref->count,ref->sizex,ref->sizey);
					}
					else if(ref->ref->itemType == Key)
					{
					sprintf(buf,"Type: Chest Item: Key Frame: %d Filename: %s Count: %d Size %d x %d ",
						ref->frame,ref->filename,ref->count,ref->sizex,ref->sizey);
					}else if(ref->ref->itemType == Lantern)
					{
					sprintf(buf,"Type: Chest Item: Lantern Frame: %d Filename: %s Count: %d Size %d x %d ",
						ref->frame,ref->filename,ref->count,ref->sizex,ref->sizey);
					}
					fputs (buf,file);
					sprintf(buf,"Location: ");
					for(y = 0; y < g_list_length(map->rooms[j].Entities);++y)
					{
						elem2 = g_list_nth(map->rooms[j].Entities,y);
						ref2 = (EntityBluePrint*)elem2->data;
						if(ref2->entType == Chest)
						{
								sprintf(temp," %.0f , %.0f ",(ref2->location.x - map->rooms[j].boundary.x )/32,(ref2->location.y - map->rooms[j].boundary.y)/32);
								strcat(buf,temp);			
						}
					}
					fputs (buf,file);
					fputs("\n",file);

				}else if(ref->entType == Portal)
				{
					sprintf(buf,"Type: Portal Location: %.0f , %.0f ",
						(ref->location.x - map->rooms[j].boundary.x )/ref->sizex,(ref->location.y - map->rooms[j].boundary.y)/ref->sizey);
					fputs (buf,file);
					fputs("\n",file);

				}else if(ref->entType == Torch)
				{
					sprintf(buf,"Type: Torch  Frame: %d Filename: %s Count: %d Size %d x %d ",
						ref->frame,ref->filename,ref->count,ref->sizex,ref->sizey);
					fputs (buf,file);
					sprintf(buf,"Location: ");
					for(y = 0; y < g_list_length(map->rooms[j].Entities);++y)
					{
						elem2 = g_list_nth(map->rooms[j].Entities,y);
						ref2 = (EntityBluePrint*)elem2->data;
						if(ref2->entType == Torch)
						{
								sprintf(temp," %.0f , %.0f ",( ref2->location.x - map->rooms[j].boundary.x)/ref2->sizex,(ref2->location.y - map->rooms[j].boundary.y )/ref2->sizey);
								strcat(buf,temp);			
						}
					}
					fputs (buf,file);
					fputs("\n",file);
				}
				else if (ref->entType == LockedDoor)
				{
					sprintf(buf,"Type: LockedDoor Frame: %d Filename: %s Count: %d Size: %d x %d ",
						ref->frame,ref->filename,ref->count,ref->sizex,ref->sizey);
					fputs (buf,file);
					sprintf(buf,"Location: ");
					for(y = 0; y < g_list_length(map->rooms[j].Entities);++y)
					{
						elem2 = g_list_nth(map->rooms[j].Entities,y);
						ref2 = (EntityBluePrint*)elem2->data;
						if(ref2->entType == LockedDoor)
						{
								sprintf(temp," %.0f , %.0f ",(ref2->location.x - map->rooms[j].boundary.x)/ref2->sizex,(ref2->location.y - map->rooms[j].boundary.y )/ref2->sizey);
								strcat(buf,temp);			
						}
					}
					fputs (buf,file);
					fputs("\n",file);
				}
			}
			if(map->rooms[j].script != NULL){
				fputs("Script: \n", file);
				if(map->rooms[j].script->type == DefeatMonsters)
				{
					sprintf(buf,"Type: DefeatFoes Filename: %s Size: %d x %d Location: %d , %d\n",
						map->rooms[j].script->filename,map->rooms[j].script->location.w,map->rooms[j].script->location.h,
						map->rooms[j].script->location.x/32,map->rooms[j].script->location.y/32);
					fputs(buf,file);
				}
			}
			fputs("\n#end\n",file);
		}

	 fclose(file);
}