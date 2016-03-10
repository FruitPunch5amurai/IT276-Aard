#ifndef __SAVE_H_
#define __SAVE_H_

typedef struct {
		int		inuse;
	int		id;
	int		whatAmI;
	int		speed;
	int		maxSpeed;
	Vec2D	position;
	Vec2D	position2;
	Vec2D	dimensions;
	char	spriteFile[256];
	int entType;
	int amIFlipped;
	int eventNumber;
	char *events[128];
	//Spirit stuff
	int spiritState;
	Uint32 nextThink;
	Vec2D  savedPlayerPos;

	int followIndex;	
	Vec2D offset;
	Entity* spirits[6];
	Entity* follow;



}PlayerSaveData;

typedef struct EventData{
	char level[128];
	char entity[128];
	char status[128];

}Events;

void PlayerSave(PlayerSaveData* ps,int eventCount,Events *events, char *filename);
void PlayerLoad(PlayerSaveData *ps, Events **events, char* filename);

#endif
