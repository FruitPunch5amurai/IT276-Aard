#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include"vectors.h"
#include "level.h"
#include"entity.h"
#include "editor_save.h"
#include "player.h"
#include "save.h"

PlayerSaveData *save;
extern Player* playerData;
extern Entity* playerEnt;
PlayerSaveData* CreatePlayerSaveData()
{
	PlayerSaveData* ps;
	ps = (PlayerSaveData*)malloc(sizeof(PlayerSaveData));
	//memcpy(ps,0,sizeof(PlayerSaveData));
	return ps;
	atexit(FreePlayerSaveData);
}
void PlayerSave(PlayerSaveData* ps,char *filename)
{
	FILE *fp;
	int i;
	GList * elem;
	ItemRef* ref;
	if(!ps)return;
	fp = fopen(filename,"wb");
	if(!fp)return;
	ps->confidence = playerData->confidence;
	ps->maxConfidence = playerData->maxConfidence;
	ps->EXP = playerData->EXP;
	ps->rescuedSpirits = playerData->rescuedSpirits;
	ps->guidingSpirits = playerData->guidingSpirits;
	ps->numOfKeys = playerData->inventory->keys;
	for(i = 0;i < g_list_length(playerData->inventory->inventory);++i)
	{
		elem = g_list_nth(playerData->inventory->inventory,i);
		ref = (ItemRef*)elem->data;
		if(ref->item != NULL)
			ps->items[i] = ref->itemType;
		else
			ps->items[i] = NONE;
	}
	if(playerData->currentItem != NULL)
		ps->currentItem = playerData->currentItem->itemType;
	else
		ps->currentItem = NONE;
	ps->position = playerEnt->position;
	strcpy(ps->mapName,GetMapName());
	//Save Map
	SaveMap(NULL);
	fwrite(ps,sizeof(PlayerSaveData),1,fp);
	//fwrite(events,sizeof(Events),ps->eventNumber,fp);

	fclose(fp);
}


void PlayerLoad(char* filename)
{
	FILE *fp;
	int i;
	fp = fopen(filename,"rb");
	if(!fp)return;
	PlayerSaveData* ps = CreatePlayerSaveData();
	fread(ps,sizeof(PlayerSaveData),1,fp);
	if(playerData == NULL)
	{
		CreatePlayerData();
		SetPlayerData();
	}
	playerData->confidence = ps->confidence;
	playerData->maxConfidence = ps->maxConfidence;
	playerData->EXP = ps->EXP;
	playerData->rescuedSpirits = ps->rescuedSpirits;
	playerData->guidingSpirits = ps->guidingSpirits;
	playerData->inventory->keys = ps->numOfKeys;
	for(i = 0;i < 12;++i)
	{
		if(ps->items[i] != NONE)
		{
			AddItemToInventory(playerData->inventory,ps->items[i]);
		}
	}
	if(ps->currentItem != NONE)
		playerData->currentItem = GetItem(playerData->inventory,ps->currentItem)->item;
	Load(ps->mapName);
	CreatePlayer(ps->position.x,ps->position.y);
	playerData->saveData = ps;
	/*if(ps->eventNumber > 0)
	{
		*events = (Events*)malloc(sizeof(Events)*ps->eventNumber);
		fread(*events,sizeof(Events),ps->eventNumber,fp);
	
	}
	*/
	fclose(fp);

}
void CreateNewPlayerSave(char *filename)
{
	FILE *fp;
	int i;
	GList * elem;
	ItemRef* ref;
	PlayerSaveData* ps = CreatePlayerSaveData();
	CreatePlayerData();
	SetPlayerData();
	playerData->saveData = ps;
	if(!ps)return;
	fp = fopen(filename,"wb");
	if(!fp)return;
	ps->confidence = 20;
	ps->maxConfidence = 30;
	ps->EXP = 0;
	ps->rescuedSpirits = 0;
	ps->guidingSpirits = 0;
	ps->numOfKeys = 0;
	for(i = 0;i < g_list_length(playerData->inventory->inventory);++i)
	{
		elem = g_list_nth(playerData->inventory->inventory,i);
		ref = (ItemRef*)elem->data;
		ps->items[i] = NONE;
	}
	ps->currentItem = NONE;
	ps->position.x = 500;
	ps->position.y = 200;
	strcpy(ps->mapName ,"level.map");

	fwrite(ps,sizeof(PlayerSaveData),1,fp);

	fclose(fp);

}
void FreePlayerSaveData()
{
	free(playerData->saveData);
	playerData->saveData = NULL;

}