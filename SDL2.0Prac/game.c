//Using SDL, SDL_image, standard IO, and strings
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "graphics.h"
#include "entity.h"
#include "sprite.h"
#include "gamepad.h"
#include "game.h"
#include <string>


SDL_Event* mainEvent = NULL;
Sprite* temp;
extern SDL_Renderer *renderer;

int main(int arc,char *argv[])
{
	Init();
	Loop();
	return 0;
}
void Init()
{
	float bgcolor[] = {1,1,1,1};
	mainEvent = new SDL_Event();
	InitSpriteList();
		Init_Graphics("Game Test",
    800,
    600,
    800,
    600,
    bgcolor);
	temp = LoadSprite("images/Sonic.png",32,42);
	temp->animation[0]->maxFrames = 5;
	temp->animation[0]->oscillate = true;
	gameState = Game;
	//InitEntityList();
	//Entity *ent = CreateEntity();
	//ent->update = &UpdateNone;

}
void Loop()
{
	int quit = 0;	
	do
	{
		quit = gameState();
		SDL_PollEvent(mainEvent);
		//handleInput(mainEvent);
		//Animate(temp->animation[0],0);
		SDL_RenderClear(renderer);
		Animate(temp->animation[0],0);
		DrawSprite(temp,0,0,GetCurrentFrame(temp->animation[0]));
		//UpdateEntities();	
		SDL_RenderPresent(renderer);
		}
	while(!quit && mainEvent->type != SDL_QUIT);
	exit(0);
	delete mainEvent;
	return;
}

int Title()
{
	printf("In title screen\n");
	//DrawTitle();
	return 0;
}

int Game()
{
	printf("In game\n");
	//DrawGame();
	return 0;
}
