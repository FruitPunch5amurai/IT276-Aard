//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include "player.h"
#include "spirit.h"
#include "graphics.h"
#include "gamepad.h"
#include "entity.h"
#include "level.h"
#include "sprite.h"
#include "gamepad.h"
#include "game.h"

Map* map;

const int AREA_WIDTH = 2400;
const int AREA_HEIGHT = 1800;
const int SCROLL_SPEED = 10;
SDL_Rect *camera;

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
	CreatePlayer();
	playerData->camera = GetCamera();
	CreateSpirit(300,300);
	map = CreateMap(32,32,75,57);
	Load("level.map",map,"images/Resources1.png");

}
/**
*@brief Main game loop
*/
void Loop()
{
	gameState = Game;
	int quit = 0;	
	do
	{
		quit = gameState();
		SDL_PollEvent(mainEvent);
		SDL_RenderClear(GetRenderer());
		handleInput(gameState);			
		DrawMap(map,1,0,0);
		DrawMap(map,2,0,0);
		DrawMap(map,3,0,0);
		DrawMap(map,0,0,0);
		SetCamera(*camera,playerEnt);
		ThinkEntities();
		UpdateEntities();
		DrawEntities();
		NextFrame();
		SDL_RenderPresent(GetRenderer());
		//FreePlayer(player);
		//CloseEntityList();
		}
	while(!quit && mainEvent->type != SDL_QUIT);
	exit(0);
	delete mainEvent;
	return;
}
/**
*@brief Sets camera
*/

void SetCamera(SDL_Rect &camera,Entity* ent)
{
    camera.w = 800;
    camera.h = 600;
	//printf("x:%d\ny:%d\n",camera.x,camera.y);
	playerData->camera = &camera;
	camera.x = (ent->position.x + ent->dimensions.x/2) - 800/2;
	camera.y = (ent->position.y + ent->dimensions.y/2) - 600/2;
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
	//printf("In game\n");
	//DrawGame();
	return 0;
}
SDL_Rect* GetCamera()
{
	return camera;
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