//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <chipmunk.h>

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
#include "editor.h"
#include "editor_panel.h"
#include "game.h"

extern Map* map;
extern Entity* playerEnt;
extern Player *playerData;

const int AREA_WIDTH = 2400;
const int AREA_HEIGHT = 1800;
const int SCROLL_SPEED = 10;

const int SCREEN_WIDTH =800;
const int SCREEN_HEIGHT =600;

Game *game;
SDL_Rect worldSize;

SDL_Rect *hotBox;
SDL_Event* mainEvent = NULL;
SDL_Texture* LightBuffer;
Sprite* temp = NULL;
Vec2D SetCameraPosition;

/*
This is where you can find all the Update,Create, and manipule data
functions for the editor
*/

/**
*@brief The Entry Point
*/
int main(int arc,char *argv[])
{
	Init();
	Loop();
	return 0;
}
/**
*@brief Initialize all the Lists,Screens, etc
*/
void Init()
{
	game = (Game*)malloc(sizeof(Game));
	memset(game,0,sizeof(Game));
	float bgcolor[] = {1,1,1,1};
	mainEvent = new SDL_Event();

	//InitMapList();
	InitSpriteList();
	Init_Graphics("Aard:GTS",
    800,
    600,
    800,
    600,
    bgcolor);
	InitEntityList();
	InitKeyData();
	game->gameState = StateTitle;
	game->font = TTF_OpenFont("Tahoma.ttf",20);
}
/**
*@brief Main game loop
*/
void Loop()
{
	int quit = 0;
	do
	{
		SDL_PollEvent(mainEvent);
		handleInput(game->gameState);	
		quit = game->gameState();
			

		}
	while(!quit && mainEvent->type != SDL_QUIT);
	graphics_close();
	exit(0);
	delete mainEvent;
	return;
}
/**
*@brief Sets camera
*/
SDL_Rect *InitHotBox()
{
	SDL_Rect *box;
	box = (SDL_Rect*)malloc(sizeof(SDL_Rect));
	box->x =  playerEnt->position.x - 200;
	box->y =  playerEnt->position.y - 100;
	box->w = .25 * SCREEN_WIDTH;
	box->h = .25 * SCREEN_HEIGHT;
	return box;
}
void UpdateHotBox()
{
	SDL_Color hp;
	hp.b = 0;hp.r = 255;hp.g = 0;hp.a = 0;
	if(playerEnt->position.x + playerEnt->dimensions.x < hotBox->x )
		hotBox->x = playerEnt->position.x + playerEnt->dimensions.x;
	if(playerEnt->position.y < hotBox->y)
		hotBox->y = playerEnt->position.y;
	if(playerEnt->position.x > hotBox->x + hotBox->w)
		hotBox->x = playerEnt->position.x - hotBox->w;
	if(playerEnt->position.y + playerEnt->dimensions.y > hotBox->y+ hotBox->h)
		hotBox->y = playerEnt->position.y+ playerEnt->dimensions.y - hotBox->h;

}
void SetCamera(SDL_Rect &camera,SDL_Rect* box)
{
	if(playerEnt != NULL){
	UpdateHotBox();
	playerEnt->camera = &camera;

    camera.w = SCREEN_WIDTH;
    camera.h = SCREEN_HEIGHT;
	//printf("x:%d\ny:%d\n",camera.x,camera.y);
	playerData->camera = &camera;
	camera.x = (box->x + box->w/2) - SCREEN_WIDTH/2;
	camera.y = (box->y + box->h/2) - SCREEN_HEIGHT/2;

	
	//Keep the camera in bounds
		if( camera.x < playerEnt->room->boundary.x)
			camera.x = playerEnt->room->boundary.x;
		if(camera.x + camera.w > playerEnt->room->boundary.x + playerEnt->room->boundary.w)
			camera.x = playerEnt->room->boundary.x + playerEnt->room->boundary.w - camera.w;
		if( camera.y < playerEnt->room->boundary.y)
			camera.y = playerEnt->room->boundary.y ;
		if(camera.y + camera.h > playerEnt->room->boundary.y + playerEnt->room->boundary.h)
			camera.y = playerEnt->room->boundary.y + playerEnt->room->boundary.h - camera.h;
		
	}
}
/**
*@brief Draws the Title Screen
*/
int StateTitle()
{
	printf("In title screen\n");
	//DrawTitle();
	return 0;
}
/**
*@brief Draws the main game
*/
int StateGame()
{
		SDL_RenderClear(GetRenderer());	
		SDL_SetRenderTarget(GetRenderer(),game->mainSceneTexture);
		SDL_RenderClear(GetRenderer());
		SDL_SetRenderTarget(GetRenderer(),NULL);
		DrawMap(1,game->camera->x,game->camera->y,game->mainSceneTexture);
		DrawMap(2,game->camera->x,game->camera->y,game->mainSceneTexture);
		DrawMap(3,game->camera->x,game->camera->y,game->mainSceneTexture);
		DrawMap(0,0,0,game->mainSceneTexture);
		ThinkEntities();
		UpdateEntities();
		DrawEntities();
		DrawMainScene();
		DrawSpecialLayer(map);
		UpdateGUI();
		SetCamera(*game->camera,hotBox);
		if(playerEnt->room->script != NULL)
			RunScript(playerEnt->room->script);
		NextFrame();
	return 0;
}
/**
*@brief Draws the main game in inventory
*/
int StateInventory()
{
		SDL_RenderClear(GetRenderer());	
		SDL_SetRenderTarget(GetRenderer(),game->mainSceneTexture);
		SDL_RenderClear(GetRenderer());
		SDL_SetRenderTarget(GetRenderer(),NULL);
		DrawMap(1,0,0,game->mainSceneTexture);
		DrawMap(2,0,0,game->mainSceneTexture);
		DrawMap(3,0,0,game->mainSceneTexture);
		DrawMap(0,0,0,game->mainSceneTexture);
		SetCamera(*game->camera,hotBox);
		DrawEntities();
		DrawMainScene();
		DrawSpecialLayer(map);
		DrawInventory(playerData->inventory);
		NextFrame();
		return 0;
}
int StateEditor()
{

	SDL_RenderClear(GetRenderer());
	UpdateMousePosition();
	UpdateWorkspace();	
	UpdateEditorPanel(MainEditorPanels); 
	DrawWorkspace();
	DrawEditorPanels(MainEditorPanels);
	NextFrame();
	return 0;

}
void DrawMainScene()
{
	SDL_RenderCopy(GetRenderer(),game->mainSceneTexture,&worldSize,&worldSize);
}
SDL_Rect* GetCamera()
{
	return game->camera;
}
void SetGameState(int (*state)())
{

	if (state == &StateGame && game->gameState == &StateTitle)
	{
		game->camera = (SDL_Rect*)malloc(sizeof(SDL_Rect));
		memset(game->camera,0,sizeof(SDL_Rect));
		game->camera->x = 0;
		game->camera->y = 0;
		Load("level.map");
		game->mainSceneTexture = SDL_CreateTexture(GetRenderer(),SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,map->w * map->tileW,map->h * map->tileH);
		LightBuffer = 
			SDL_CreateTexture(GetRenderer(),SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,
			map->w * map->tileW,map->h * map->tileH);
		SDL_SetTextureBlendMode(LightBuffer,SDL_BLENDMODE_BLEND);	
		worldSize.x = worldSize.y = 0;
		worldSize.w =map->w * map->tileW;
		worldSize.h = map->h * map->tileH;
		CreatePlayerData();
		CreatePlayer(400,400);
		SetPlayerData();
		playerData->camera = GetCamera();
		hotBox = InitHotBox();
	}
	else if (state == &StateEditor && game->gameState == &StateTitle)
	{
		game->camera = (SDL_Rect*)malloc(sizeof(SDL_Rect));
		memset(game->camera,0,sizeof(SDL_Rect));
		game->camera->x = 0;
		game->camera->y = 0;
	}
	game->gameState = state;
}
