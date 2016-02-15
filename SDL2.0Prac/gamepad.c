#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <string>
#include "gamepad.h"


extern int (*gameState)();
extern int Game();
extern int Title();

void handleInput(SDL_Event *e)
{
	if(gameState = Game)
	{
		printf("This is the gameState event Handler\n");
	}

}


