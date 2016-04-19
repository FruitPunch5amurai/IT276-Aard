#ifndef __GAME_H
#define __GAME_H

#include "player.h"
#include "spirit.h"
#include "vectors.h"
#include "graphics.h"
#include "enemy.h"
#include "gamepad.h"
#include "entity.h"
#include "level.h"
#include "sprite.h"
#include "obj.h"
#include "collision.h"
#include "LList.h"
#include "gamepad.h"


typedef struct GameData{
	//int (*gameState) ();
	//SDL_Texture* mainSceneTexture;
	//SDL_Event* mainEvent;
	//TTF_Font *font;
	//SDL_Rect *camera;
}Game;


Vec2D GetCameraSize();
void SetCameraSize(Vec2D size);
SDL_Rect* GetCamera();	
void DrawTitle();
void DrawGame();
void DrawMainScene();
void Init();
void Loop();
void SetCamera(SDL_Rect &camera,SDL_Rect *box);
void UpdateHotBox();
SDL_Rect *InitHotBox();
void SetGameState(int (*state)());

void SetUpMap(Map *map,FILE *file);


extern Game *game;
#endif
