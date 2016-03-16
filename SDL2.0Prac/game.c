//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include "player.h"
#include "spirit.h"
#include "graphics.h"
#include "enemy.h"
#include "gamepad.h"
#include "entity.h"
#include "level.h"
#include "sprite.h"
#include "gamepad.h"
#include "game.h"

extern Map* map;

const int AREA_WIDTH = 2400;
const int AREA_HEIGHT = 1800;
const int SCROLL_SPEED = 10;

const int SCREEN_WIDTH =800;
const int SCREEN_HEIGHT =600;
SDL_Rect *camera;
SDL_Rect *hotBox;

extern Entity* playerEnt;
extern Player *playerData;

SDL_Event* mainEvent = NULL;
Sprite* temp = NULL;

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
	float bgcolor[] = {1,1,1,1};
	mainEvent = new SDL_Event();
	camera = (SDL_Rect*)malloc(sizeof(camera));
	memset(camera,0,sizeof(SDL_Rect));
	camera->x = 0;
	camera->y = 0;
	InitSpriteList();
	Init_Graphics("Game Test",
    800,
    600,
    800,
    600,
    bgcolor);
	InitEntityList();
	InitKeyData();
	Load("level.map","images/Resources1.png");
	CreatePlayer(400,400);
	playerData->camera = GetCamera();
	hotBox = InitHotBox();
}
/**
*@brief Main game loop
*/
void Loop()
{
	gameState = Title;
	int quit = 0;	
	do
	{
		quit = gameState();
		SDL_PollEvent(mainEvent);
		handleInput(gameState);		

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
		UpdateHotBox();

    camera.w = SCREEN_WIDTH;
    camera.h = SCREEN_HEIGHT;

	//printf("x:%d\ny:%d\n",camera.x,camera.y);
	playerData->camera = &camera;
	camera.x = (box->x + box->w/2) - SCREEN_WIDTH/2;
	camera.y = (box->y + box->h/2) - SCREEN_HEIGHT/2;

	
	//Keep the camera in bounds
    if( camera.x < 0 )
    { 
        camera.x = 0;
    }
    if( camera.y < 0 )
    {
        camera.y = 0;
    }
    if( camera.x > AREA_WIDTH - camera.w )
    {
        camera.x = AREA_WIDTH - camera.w;
    }
    if( camera.y > AREA_HEIGHT - camera.h )
    {
        camera.y = AREA_HEIGHT - camera.h;
    }
	
}
/**
*@brief Draws the Title Screen
*/
int Title()
{
	printf("In title screen\n");
	//DrawTitle();
	return 0;
}
/**
*@brief Draws the main game
*/
int Game()
{
		SDL_RenderClear(GetRenderer());	
		DrawMap(1,0,0);
		DrawMap(2,0,0);
		DrawMap(3,0,0);
		DrawMap(0,0,0);
		SetCamera(*camera,hotBox);
		ThinkEntities();
		UpdateEntities();
		DrawEntities();
		NextFrame();
		SDL_RenderPresent(GetRenderer());
	return 0;
}
SDL_Rect* GetCamera()
{
	return camera;
}
void SetGameState(int (*state)())
{
	gameState = state;
}
/*
Vec2D GetCameraPosition()
{
	return CreateVec2D(camera.x,camera.y);
}
Vec2D GetCameraSize()
{
	return CreateVec2D(camera.w,camera.h);
}

void SetCameraSize(Vec2D size)
{
	camera.w = size.x;
	camera.y = size.y;
}
*/