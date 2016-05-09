#ifndef __SAVE_H_
#define __SAVE_H_
#include "items.h"
typedef struct {
	int confidence;
	int maxConfidence;
	int EXP;
	int rescuedSpirits;
	int guidingSpirits;
	int numOfKeys;
	ItemType items[12];
	ItemType currentItem;
	Vec2D	position;
	Vec2D	position2;
	char mapName[255];
	//char	spriteFile[256];
	//int eventNumber;
	//char *events[128];

}PlayerSaveData;

typedef struct EventData{
	char level[128];
	char entity[128];
	char status[128];

}Events;
PlayerSaveData* CreatePlayerSaveData();
void CreateNewPlayerSave(char *filename);
void PlayerSave(PlayerSaveData* ps,char *filename);
void PlayerLoad(char* filename);
void FreePlayerSaveData();
#endif
