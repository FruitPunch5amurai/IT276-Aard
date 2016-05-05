#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "sprite.h"
#include "entity.h"

typedef struct lightData{
	Sprite* sprite;
	Entity* source;
	SDL_Rect src;


}LightSource;

LightSource* CreateLight(Entity* source);
void DrawLights();
void InitLightList();
void FreeLight(LightSource* l);
void CloseLightList();

#endif