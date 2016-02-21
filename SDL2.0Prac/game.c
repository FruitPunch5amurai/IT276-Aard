//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include "player.h"
#include "graphics.h"
#include "gamepad.h"
#include "entity.h"
#include "level.h"
#include "sprite.h"
#include "gamepad.h"
#include "game.h"

Map* map;

const int AREA_WIDTH = 1600;
const int AREA_HEIGHT = 1200;

SDL_Rect *camera;

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

	map = CreateMap(32,32,25,19);
	Load("level.map",map,"images/Resources.png");

}
/**
*@brief Main game loop
*/
void Loop()
{
	gameState = Game;
	Player* player;
	player = CreatePlayer();
	player->camera = GetCamera();
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
		SetCamera(*camera,player);
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

void SetCamera(SDL_Rect &camera,Player* player)
{
    camera.w = 800;
    camera.h = 600;
	printf("x:%d\ny:%d\n",camera.x,camera.y);

	camera.x = (player->self->position.x + player->self->dimensions.x/2) - 800/2;
	camera.y = (player->self->position.y + player->self->dimensions.y/2) - 600/2;
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
