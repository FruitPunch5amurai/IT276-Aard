#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include "entity.h"
#include "player.h"
#include "gamepad.h"

KeyData * keyData = NULL;
extern SDL_Event* mainEvent;
extern Entity *playerEnt;
extern Player *playerData;
extern void SetGameState(int (*state)());


void InitKeyData()
{
	keyData = (KeyData*)malloc(sizeof(KeyData));
	memset(keyData,0,sizeof(KeyData));
	atexit(CloseKeyData);
}
void CloseKeyData()
{
	 free(keyData);
	 keyData = NULL;
}
/**
*@brief handles input from keyboard depending on the gameState
*@brief handles player velocity
*@param Takes in the game state
*/
void handleInput(int (*gameState)())
{
	if(gameState == &StateGame)
	{
		//printf("This is the Game\n");
		if(playerEnt != NULL){
		
		if(mainEvent->type == SDL_KEYDOWN)	
		{
			switch(mainEvent->key.keysym.sym)
			{
				
			case SDLK_LEFT:
				
				playerEnt->velocity.x = -playerEnt->speed;
				playerEnt->facing.x = -1;
				playerEnt->facing.y = 0;
				playerEnt->flipped = SDL_FLIP_HORIZONTAL;
				keyData->ArrowKeyLeft = 1;
				break;
			case SDLK_RIGHT:
				playerEnt->facing.x = 1;
				playerEnt->facing.y = 0;
				playerEnt->velocity.x = playerEnt->speed;
				playerEnt->flipped = SDL_FLIP_NONE;
				keyData->ArrowKeyRight = 1;
				break;
			case SDLK_UP:
				playerEnt->velocity.y = -playerEnt->speed;
				playerEnt->facing.x = 0;
				playerEnt->facing.y = 1;
				keyData->ArrowKeyUp = 1;
				break;
			case SDLK_DOWN:
				playerEnt->velocity.y = playerEnt->speed;
				playerEnt->facing.x = 0;
				playerEnt->facing.y = -1;
				keyData->ArrowKeyDown = 1;
				break;
			case SDLK_q:
				printf("q pressed");keyData->Q = 1;
				break;
			case SDLK_w:
				printf("w pressed");keyData->W = 1;
				break;
			case SDLK_e:
				printf("e pressed");keyData->E = 1;
				break;
			case SDLK_r:
				printf("r pressed");keyData->R = 1;
				break;
			case SDLK_SPACE:
				printf("Spacebar pressed\n");keyData->Spacebar = 1;
				break;
			case SDLK_RETURN:
				ItemRef* ref;
				printf("Enter pressed\n");
				keyData->Enter = 1;
				SetGameState(StateInventory);
				playerData->inventory->cursor->ref = 
					g_list_nth(playerData->inventory->inventory,0);
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
				playerEnt->velocity.x = 0;keyData->ArrowKeyLeft = 0;break;
			case SDLK_RIGHT:
				playerEnt->velocity.x = 0;keyData->ArrowKeyRight = 0;break;
			case SDLK_UP:
				playerEnt->velocity.y = 0;keyData->ArrowKeyUp = 0;break;
			case SDLK_DOWN:
				playerEnt->velocity.y = 0;keyData->ArrowKeyDown = 0;break;
			case SDLK_q:
				keyData->Q = 0;
				break;
			case SDLK_w:
				keyData->W = 0;
				break;
			case SDLK_e:
				keyData->E = 0;
				break;
			case SDLK_r:
				keyData->R = 0;
				break;
			case SDLK_SPACE:
				keyData->Spacebar = 0;
				break;
			default:
				break;
			}
		}
	}
}
	//TODO fix gamestate change!
	if(gameState == &StateTitle)
	{
		if(mainEvent->type == SDL_KEYDOWN){
			if(mainEvent->key.keysym.sym == SDLK_SPACE)
				SetGameState(StateGame); 
		}
	}
	if(gameState == &StateInventory)
	{
		//printf("This is the Game\n");
		if(playerEnt != NULL){
		
		if(mainEvent->type == SDL_KEYDOWN)	
		{
			switch(mainEvent->key.keysym.sym)
			{
			case SDLK_RETURN:
				if(keyData->Enter != 1)
					SetGameState(StateGame);
				keyData->Enter = 1;
				playerEnt->velocity.x = 0;
				keyData->ArrowKeyDown = 0;
				keyData->ArrowKeyLeft = 0;
				keyData->ArrowKeyRight = 0;
				keyData->ArrowKeyUp = 0;
				break;
			case SDLK_RIGHT:
				keyData->ArrowKeyRight = 1;
				playerData->inventory->cursor->ref = g_list_next(playerData->inventory->cursor->ref);
				if(playerData->inventory->cursor->ref == NULL)
					playerData->inventory->cursor->ref = g_list_last(playerData->inventory->inventory);
				break;
			case SDLK_LEFT:
				keyData->ArrowKeyLeft = 1;
				playerData->inventory->cursor->ref = g_list_previous(playerData->inventory->cursor->ref);
				if(playerData->inventory->cursor->ref == NULL)
					playerData->inventory->cursor->ref = g_list_first(playerData->inventory->inventory);
				break;
			case SDLK_SPACE:
				//Unequip Item
 				if(playerData->currentItem != NULL){
					FreeEntity(playerData->currentItem->self);
					playerData->currentItem->self = NULL;
					playerData->currentItem->isEquiped = 0;
				}
				//Equip Item cursor is pointing to
				playerData->currentItem = ((ItemRef*)playerData->inventory->cursor->ref->data)->item;

			default:
				break;
			}
		}
		if(mainEvent->type == SDL_KEYUP)	
		{
			switch(mainEvent->key.keysym.sym)
			{
			case SDLK_RETURN:
				keyData->Enter = 0;
				break;
			case SDLK_LEFT:
				keyData->ArrowKeyLeft = 0;
				break;
			case SDLK_RIGHT:
				keyData->ArrowKeyRight = 0;
				break;
			default:
				break;
			}
		}
		}
	}
}


