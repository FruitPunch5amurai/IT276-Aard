#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include "player.h"
#include "gamepad.h"

extern SDL_Event* mainEvent;
extern int Game();
extern int Title();
extern Entity *playerEnt;

/**
*@brief handles input from keyboard depending on the gameState
*@brief handles player velocity
*@param Takes in the game state
*/
void handleInput(int (*gameState)())
{
	if(gameState == &Game)
	{
		//printf("This is the Game\n");
		if(playerEnt != NULL){
		if(mainEvent->type == SDL_KEYDOWN)	
		{
			switch(mainEvent->key.keysym.sym)
			{
				
			case SDLK_LEFT:
				printf("ArrowKeyLeft Key Pressed\n");
				playerEnt->velocity.x = -playerEnt->speed;
				if(!playerEnt->amIFlipped)
				{
					playerEnt->amIFlipped = 1;
					playerEnt->flipped = SDL_FLIP_HORIZONTAL;
				}
				break;
			case SDLK_UP:
				printf("ArrowKeyUp Key Pressed\n");playerEnt->velocity.y = -playerEnt->speed;break;
			case SDLK_DOWN:
				printf("ArrowKeyDown Key Pressed\n");playerEnt->velocity.y = playerEnt->speed;break;
			case SDLK_RIGHT:
				printf("ArrowKeyRight Key Pressed\n");
				playerEnt->velocity.x = playerEnt->speed;
				if(playerEnt->amIFlipped)
				{
					playerEnt->amIFlipped = 0;
					playerEnt->flipped = SDL_FLIP_NONE;
				}
				break;
			default:
				break;
			}
		}
		if (mainEvent->type == SDL_KEYUP)
		{
			switch(mainEvent->key.keysym.sym)
			{
			case SDLK_LEFT:
				printf("ArrowKeyLeft Key Released\n");playerEnt->velocity.x = 0;break;
			case SDLK_UP:
				printf("ArrowKeyUp Key Released\n");playerEnt->velocity.y = 0;break;
			case SDLK_DOWN:
				printf("ArrowKeyDown Key Released\n");playerEnt->velocity.y = 0;break;
			case SDLK_RIGHT:
				printf("ArrowKeyRight Key Released\n");playerEnt->velocity.x = 0;break;
			default:
				break;
			}
		}
	}
}
	//TODO fix gamestate change!
	if(gameState == &Title)
	{
		if(mainEvent->type == SDL_KEYDOWN){
			if(mainEvent->key.keysym.sym == SDLK_SPACE)
				gameState = Game; 
		}
	}

}


