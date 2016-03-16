#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdlib.h>
#include <string>
#include "sprite.h"
#include "graphics.h"
#include "vectors.h"
#include "LList.h"
#include "spirit.h"
#include "gamepad.h"
#include "player.h"

SDL_Color hp;
SDL_Color hp2;
SDL_Color whiteFont;
Player *playerData = NULL;
Entity* playerEnt= NULL;
extern Link *l;
extern ELink *SpiritLink;
extern Map *map;
extern KeyData *keyData;
char numOfSpirits[256];

/**
*@brief Loads in Data for AnimationData for the player from a .txt
*/
void LoadPlayerAnimations(Entity* ent)
{
	int j,i=0;
	FILE *file;
	file = fopen("PlayerAnimationData.txt","r");

	if(!file)
	{
		printf("Unable to read AnimationData.txt");
		return;
	}
	while(!feof(file))
	{
		fscanf(file,"%d",&j);
		ent->sprite->animation[i]->currentFrame = j;
		fscanf(file,"%d",&j);
		ent->sprite->animation[i]->startFrame = j;
		fscanf(file,"%d",&j);
		ent->sprite->animation[i]->maxFrames = j;
		fscanf(file,"%d",&j);
		ent->sprite->animation[i]->oscillate = j;

		i++;
	}
}
/**
*@brief Creates and Allocates memory for the player. Then initializes all the
*@brief data members for Player
*/
void CreatePlayer(int x, int y)
{
	playerEnt = CreateEntity();

	playerData = (Player*)malloc(sizeof(Player));
	memset(playerData,0,sizeof(Player));

	//Data for Idle Animation
	playerEnt->sprite= LoadSprite("images/Sonic.png",32,42);
	LoadPlayerAnimations(playerEnt);
	playerEnt->whatAmI = 0;
	playerEnt->dimensions.x = playerEnt->sprite->w;
	playerEnt->dimensions.y = playerEnt->sprite->h;
	playerEnt->facing.x = 0;
	playerEnt->facing.y = 0;
	playerEnt->position.x = x;
	playerEnt->position.y = y;
	playerEnt->hitBox.x = playerEnt->position.x;
	playerEnt->hitBox.y = playerEnt->position.y;
	playerEnt->hitBox.w = playerEnt->sprite->w;
	playerEnt->hitBox.h= playerEnt->sprite->h;
	playerEnt->atkBox.x = 0;
	playerEnt->atkBox.y = 0;
	playerEnt->atkBox.w = 0;
	playerEnt->atkBox.h = 0;
	playerEnt->position2.x = playerEnt->position.x+playerEnt->dimensions.x-1;
	playerEnt->position2.y = playerEnt->position.y+playerEnt->dimensions.y-1;
	playerEnt->speed = 5;

	/*Set the PlayerData*/
	playerData->confidence = 20;
	playerData->maxConfidence = 30;
	playerData->rescuedSpirits = 0;
	playerData->guidingSpirits = 0;
	playerData->abandonSpirits = 0;
	for(int i = 0;i < 4;i++)
	{
		playerData->abilities[i].cooldown = 0;
		playerData->abilities[i].maxCooldown = 10;
		playerData->abilities[i].inuse = 0;
		playerData->abilities[i].unlocked = 1; 
	}
	playerData->font = TTF_OpenFont("Tahoma.ttf",20);
	playerData->textRect.x = 20;
	playerData->textRect.y = 40;
	playerData->textRect.w = 10;
	playerData->textRect.h = 10;

	playerEnt->update = &UpdatePlayer;
	playerEnt->draw = &DrawPlayer;
	playerEnt->think = &ThinkPlayer;
	playerEnt->touch = &TouchPlayer;

	SetElement(l,playerEnt);
	Insert(SpiritLink,playerEnt);
	Next(SpiritLink);
}
/**
*@brief Draws specified Entity to main renderer
*/
void DrawPlayer(Entity* ent)
{
	DrawEntity(ent,ent->currentAnimation,ent->position.x,ent->position.y);
}
/**
*@brief Updates specified Entity
*/
void UpdatePlayer(Entity *ent)
{
	UpdateGUI();
	ExecuteSkill();
	if(playerData->confidence <= 0)
		printf("Player is Dead");
	ent->spiritIndex = 0;
	ent->hitBox.x =ent->position.x;
	ent->hitBox.y =ent->position.y;
	
	if(ent->velocity.x != 0 || ent->velocity.y != 0)
		ent->currentAnimation= 1;
	else
		ent->currentAnimation = 0;
	Vec2DAdd(ent->position,ent->position,OverlapsMap(map,ent));
	Vec2DAdd(ent->position,ent->position,ent->velocity);
	EntityIntersectAll(ent);
	AttackBoxIntersectAll(ent);
}

void ThinkPlayer(Entity *ent)
{
	if(playerEnt->nextThink < SDL_GetTicks())
	{
		for(int i = 0;i < 4;++i){
			if(playerData->abilities[0].cooldown > 0){
				playerData->abilities[0].cooldown--;
				playerData->abilities[0].inuse = 0;
			}
		}
		playerEnt->nextThink = SDL_GetTicks() +1000;
	}
	playerData->guidingSpirits = SpiritLink->count;
	Vec2DAdd(ent->position,ent->position,OverlapsMap(map,ent));
	ent->atkBox.x = 0;
	ent->atkBox.y = 0;
	ent->atkBox.w = 0;
	ent->atkBox.h = 0;
	if(playerData->confidence <= 0)
	{
		FreePlayer();
	}
}
void TouchPlayer(Entity *ent,Entity *other)
{

	if(ent->atkBox.w == 0 && ent->atkBox.h == 0)
	{
	if(other->whatAmI == 3)
	{
		moveToEnd(SpiritLink);
		for(int i = 1;i < SpiritLink->count;++i)
		{
			FreeSpirit(SpiritLink->curr->curr);
			playerData->confidence = playerData->confidence < playerData->maxConfidence 
				? playerData->confidence+3 : playerData->maxConfidence;
			map->numOfSpirits--;
		}

	}else if(other->whatAmI == 2)
	{
		other->position2 = other->velocity;
		Vec2DNormalize(&other->position2);
		(ent->velocity.x == 0 && ent->velocity.y == 0)?
			Vec2DAdd(ent->position,ent->position,
				CreateVec2D(other->speed*6*other->position2.x,other->speed*6*other->position2.y)):
			Vec2DAdd(ent->position,ent->position,
				-CreateVec2D(ent->speed * ent->velocity.x ,ent->speed* ent->velocity.y));

	}
		
	}else{
		if(other->whatAmI == 2)
		{
		//whipATK
			other->knockback = 1;
			other->velocity = CreateVec2D(other->speed*2*playerEnt->facing.x,-other->speed*2*playerEnt->facing.y);
		}
	}
}
/**
*@brief Deallocates memory for player
*/
void FreePlayer()
{
	FreeEntity(playerEnt);
}

/**
*@brief Returns player if not null
*/
Entity *GetPlayer()
{
	if(!playerEnt)
	{
		return playerEnt;
	}
	printf("Player does not exist.");
}
void RenderHPBar(int x, int y, int w, int h, float Percent, SDL_Color FGColor, SDL_Color BGColor) { 
	
	hp.b = 0;hp.r = 0;hp.g = 0;hp.a = 0;
	hp2.b= 0; hp2.r = 225; hp2.g = 215;hp2.a = 0;
	
   Percent = Percent > 1.f ? 1.f : Percent < 0.f ? 0.f : Percent; 
   Percent < .5f ? hp.r = 255,hp.g = 0,hp.b = 0 : hp.r = 0,hp.g = 255,hp.b = 0;
   SDL_Rect bgrect = { x, y, w+2, h }; 
   SDL_SetRenderDrawColor(GetRenderer(), BGColor.r, BGColor.g, BGColor.b, BGColor.a); 
   SDL_RenderDrawRect(GetRenderer(), &bgrect); 
   SDL_SetRenderDrawColor(GetRenderer(), FGColor.r, FGColor.g, FGColor.b, FGColor.a); 
   int pw = (int)((float)w * Percent); 
   SDL_Rect fgrect = { x+1, y+1, pw, h-2 }; 
   SDL_RenderFillRect(GetRenderer(), &fgrect); 
   } 
void UpdateGUI()
{
	whiteFont.r = 255,whiteFont.g = 255,whiteFont.b = 255,whiteFont.a = 0;
	itoa(map->numOfSpirits,numOfSpirits,10);
	RenderHPBar( 10, 10, playerData->maxConfidence,25,(float)playerData->confidence/playerData->maxConfidence, hp ,hp2);
	RenderFont(numOfSpirits,playerData->textRect,playerData->font,&whiteFont);
}
void SkillWhip()
{
	SDL_Rect rect; 
	rect.w = playerEnt->facing.x < 0 || playerEnt->facing.x > 0 ? 132 : 50;
	rect.h = playerEnt->facing.y < 0 || playerEnt->facing.y > 0 ? 132 : 50;
	rect.x = playerEnt->facing.x < 0 && playerEnt->facing.x != 0?
		playerEnt->position.x - playerData->camera->x - rect.w+playerEnt->dimensions.x:playerEnt->position.x - playerData->camera->x;
	rect.y = playerEnt->facing.y < 0 && playerEnt->facing.y != 0?
		playerEnt->position.y - playerData->camera->y:playerEnt->position.y - playerData->camera->y - rect.h+playerEnt->dimensions.y;
	playerEnt->atkBox.x = playerEnt->facing.x < 0 && playerEnt->facing.x != 0?
		playerEnt->position.x - rect.w+playerEnt->dimensions.x:playerEnt->position.x;
	playerEnt->atkBox.y = playerEnt->facing.y < 0 && playerEnt->facing.y != 0?
		playerEnt->position.y :playerEnt->position.y- rect.h+playerEnt->dimensions.y;
	playerEnt->atkBox.w = rect.w;
	playerEnt->atkBox.h = rect.h;
	SDL_SetRenderDrawColor(GetRenderer(), whiteFont.r, whiteFont.g, whiteFont.b, whiteFont.a); 
	SDL_RenderDrawRect(GetRenderer(), &rect);
}
void SkillRetrieve()
{
		moveToEnd(SpiritLink);
		for(int i = SpiritLink->count;i > 0;--i)
		{
			SpiritLink->curr->curr->position = playerEnt->position;
			Prev(SpiritLink);
		}
}
void SkillNEVERGONNAGIVEYOUUP()
{
	Entity* timer = CreateTimer(4);
	moveToEnd(SpiritLink);
			for(int i = SpiritLink->count;i > 1;--i)
		{
			SpiritLink->curr->curr->timer = timer;
			SpiritLink->curr->curr->immunity = 1;
			Prev(SpiritLink);
		}
}
void ExecuteSkill()
{
	if(keyData->Q == 1 && playerData->abilities[0].inuse == 0 && playerData->abilities[0].cooldown == 0)
	{
		playerData->abilities[0].inuse = 1;
		playerData->abilities[0].cooldown = 10;
		printf("Whip used:%d\n" ,playerData->abilities[0].cooldown);
		SkillWhip();
	}
	if(keyData->W == 1 && playerData->abilities[1].inuse == 0 && playerData->abilities[1].cooldown == 0)
	{
		playerData->abilities[1].inuse = 1;
		playerData->abilities[1].cooldown = 10;
		printf("Retrieve used:%d\n" ,playerData->abilities[0].cooldown);
		SkillRetrieve();
	}
		if(keyData->E == 1 && playerData->abilities[2].inuse == 0 && playerData->abilities[2].cooldown == 0)
	{
		playerData->abilities[2].inuse = 1;
		playerData->abilities[2].cooldown = 0;
		printf("Immune used:%d\n" ,playerData->abilities[0].cooldown);
		SkillNEVERGONNAGIVEYOUUP();
	}
}

Entity *CreateTimer(Uint8 time)
{
	Entity *timer;
	timer= CreateEntity();
	timer->temp = time;
	timer->nextThink = SDL_GetTicks() + 1000;
	timer->think = &ThinkTimer;
	return timer;
}

void ThinkTimer(Entity *ent)
{
	if(ent->nextThink < SDL_GetTicks() && ent->temp > 0){
		ent->nextThink = SDL_GetTicks() + 1000;
		ent->temp -=1;
		printf("%d",ent->temp);
	}
	if(ent->temp == 0)
	{
		FreeEntity(ent);
	}
}
