#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include"vectors.h"
#include"entity.h"
#include "save.h"

PlayerSaveData *save;

void PlayerSave(PlayerSaveData *ps, Events *events, char *filename)
{
	FILE *fp;
	int i;
	if(!ps)return;
	fp = fopen(filename,"wb");
	if(!fp)return;

	fwrite(ps,sizeof(PlayerSaveData),1,fp);
	fwrite(events,sizeof(Events),ps->eventNumber,fp);

	fclose(fp);
}


void PlayerLoad(PlayerSaveData *ps, Events **events, char* filename)
{
	FILE *fp;
	
	fp = fopen(filename,"rb");
	
	fread(ps,sizeof(PlayerSaveData),1,fp);
	if(ps->eventNumber > 0)
	{
		*events = (Events*)malloc(sizeof(Events)*ps->eventNumber);
		fread(*events,sizeof(Events),ps->eventNumber,fp);
	
	}
	
	fclose(fp);

}
