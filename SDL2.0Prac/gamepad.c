#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include "player.h"
#include "gamepad.h"

extern SDL_Event* mainEvent;
extern int Game();
extern int Title();
extern Player *player;


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
		if(player != NULL){
		if(mainEvent->type == SDL_KEYDOWN)	
		{
			switch(mainEvent->key.keysym.sym)
			{
				
			case SDLK_LEFT:
				printf("ArrowKeyLeft Key Pressed\n");
				player->self->velocity.x = -player->playerSpeed;
				if(!player->self->amIFlipped)
				{
					player->self->amIFlipped = 1;
					player->self->flipped = SDL_FLIP_HORIZONTAL;
				}
				break;
			case SDLK_UP:
				printf("ArrowKeyUp Key Pressed\n");player->self->velocity.y = -player->playerSpeed;break;
			case SDLK_DOWN:
				printf("ArrowKeyDown Key Pressed\n");player->self->velocity.y = player->playerSpeed;break;
			case SDLK_RIGHT:
				printf("ArrowKeyRight Key Pressed\n");
				player->self->velocity.x = player->playerSpeed;
				if(player->self->amIFlipped)
				{
					player->self->amIFlipped = 0;
					player->self->flipped = SDL_FLIP_NONE;
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
				printf("ArrowKeyLeft Key Released\n");player->self->velocity.x = 0;break;
			case SDLK_UP:
				printf("ArrowKeyUp Key Released\n");player->self->velocity.y = 0;break;
			case SDLK_DOWN:
				printf("ArrowKeyDown Key Released\n");player->self->velocity.y = 0;break;
			case SDLK_RIGHT:
				printf("ArrowKeyRight Key Released\n");player->self->velocity.x = 0;break;
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


