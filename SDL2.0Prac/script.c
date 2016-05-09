#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <glib.h>
#include "obj.h"
#include "entity.h"
#include "vectors.h"
#include "script.h"
extern Entity* playerEnt;
extern Entity* EntityList;
extern Map* map;
extern int MAX_ENTITIES;

Script* CreateScript(ScriptType type, char *filename,int RoomId,int x, int y,int w,int h)
{
	int i,j;
	Script *script;
	script = (Script*)malloc(sizeof(Script));
	memset(script,0,sizeof(Script));

	script->type = type;
	script->count = 0;
	strcpy(script->filename,filename);
	script->activated = 0;
	script->location.x =x;
	script->location.y =y;
	script->location.w = w;
	script->location.h = h;
	if(script->type == DefeatMonsters)
	{
		int j = 0;
		script->update = &DefeatFoes;
		
	}
	if(RoomId == -1)
		map->rooms[0].script = script;
	else
		map->rooms[RoomId].script = script;
	return script;
}
int DefeatFoes(Script* script)
{
	int i;
	if(script->activated == 0){
		if(DistanceBetweenGreaterThan2D(playerEnt->savedPlayerPos,playerEnt->position,100)){
			script->ent = CreateObject(CreateVec2D(script->location.x,script->location.y), script->location.w,  script->location.h, Door,70,
							script->filename,NULL,NULL);
			Mix_PlayChannel(-1,(Mix_Chunk*)g_hash_table_lookup(script->ent->sounds,"Close"),0);
			script->activated = 1;
		}
	}
	if(script->activated == 1){
		for(i = 0; i < MAX_ENTITIES;++i)
		{
			if(EntityList[i].whatAmI != Enemy)
				continue;
			
			printf("Script Not fullfilled\n");
			return 0;
		}
		
		for(i = 0; i < MAX_ENTITIES;++i)
		{
			if(EntityList[i].whatAmI == LockedDoor || EntityList[i].whatAmI == Door)
			{
				EntityList[i].whatAmI = UnlockedDoor;
				(*EntityList[i].free)(&EntityList[i]);
			}
		}
		Mix_PlayChannel(-1,(Mix_Chunk*)g_hash_table_lookup(script->ent->sounds,"Open"),0);
		Mix_PlayChannel(-1,(Mix_Chunk*)g_hash_table_lookup(script->ent->sounds,"Secret"),0);
		printf("Script fullfilled\n");
		return 1;
	}
}
void RunScript(Script* script)
{
	(*script->update)(script);
}
