#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdlib.h>
#include <string>
#include <jansson.h>
#include "game.h"

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
void Stun();
/**
*@brief Loads in Data for AnimationData for the player using Jansson
*/
void LoadPlayerAnimations(Entity* ent)
{
	
	json_t *root;
	json_error_t err;
	root = json_load_file("AnimationData.json",0,&err);
	if(!root)
	{
		fprintf(stderr,"error: on line %d : %s\n", err.line,err.text);
		exit(1);
	}

		json_t *data , *Entity, *Animations;
		char *ENT ,*filepath; 
		int frameW, frameH;
		data = json_object_get(root,"AnimationData");
		Entity = json_array_get(data,0);
		json_unpack(Entity,"{s:s,s:s,s:i,s:i}","Entity",&ENT,"filepath",&filepath,"frame-width",&frameW,"frame-height",&frameH);
		//load sprite
		playerEnt->sprite = LoadSprite(filepath,frameW,frameH);
		data = json_object_get(Entity,"Animations");
			if(!json_is_array(data))
			{
				fprintf(stderr, "error: commit %d: Not an array\n", 0);
				json_decref(root);
				exit(1);
			}
		//load animations
		for(int i = 0;i < json_array_size(data);++i)
		{
			Animations = json_array_get(data,i);	
			if(!json_is_object(Animations))
			{
				fprintf(stderr, "error: commit %d: Not an object\n", 0);
				json_decref(root);
				exit(1);
			}

			playerEnt->sprite->animation[i].startFrame = json_number_value(json_object_get(Animations,"start-frame"));
			playerEnt->sprite->animation[i].currentFrame = playerEnt->sprite->animation[i].startFrame;
			playerEnt->sprite->animation[i].maxFrames = json_number_value(json_object_get(Animations,"max-frames"));
			playerEnt->sprite->animation[i].oscillate = json_number_value(json_object_get(Animations,"oscillate"));
			playerEnt->sprite->animation[i].holdFrame = 0;
			playerEnt->sprite->animation[i].frameInc = 1;
			playerEnt->sprite->animation[i].frameRate = 100;
			playerEnt->sprite->animation[i].frameInc = 1;
			if(json_object_get(Animations,"held-frame"))
				playerEnt->sprite->animation[i].heldFrame = json_number_value(json_object_get(Animations,"held-frame"));
			else
				playerEnt->sprite->animation[i].heldFrame = -1;
		}
		playerEnt->currentAnimation = 0;
		json_decref(root);
}
void CreatePlayerData()
{
	playerData = (Player*)malloc(sizeof(Player));
	memset(playerData,0,sizeof(Player));

}
void SetPlayerData()
{
	GList * elem;
	ItemRef* ref;
	int n;
	Vec2D pos;
		/*Set the PlayerData*/
	playerData->inventory = InitInventory();
	playerData->confidence = 20;
	playerData->maxConfidence = 30;
	playerData->rescuedSpirits = 0;
	playerData->guidingSpirits = 0;
	playerData->abandonSpirits = 0;
	playerData->inventory->keys = 1;
	playerData->EXP = 0;
	for(int i = 0;i < 4;i++)
	{
		playerData->abilities[i].cooldown = 0;
		playerData->abilities[i].maxCooldown = 10;
		playerData->abilities[i].inuse =0;
		playerData->abilities[i].unlocked = 1; 
	}
	playerData->font = TTF_OpenFont("Tahoma.ttf",20);
	playerData->textRect.x = 20;
	playerData->textRect.y = 40;
	playerData->textRect.w = 10;
	playerData->textRect.h = 10;
	//playerData->inventory[0].inventory->item = LoadItem(Lantern);
 	for(n = 0; n < g_list_length(playerData->inventory->inventory);++n)
	{
		elem = g_list_nth(playerData->inventory->inventory,n);
		ref = (ItemRef*)elem->data;
		if(!ref->item)
		{
			pos = ref->pos;
			ref = LoadItem(Lantern);
			ref->pos = CreateVec2D(pos.x,pos.y);
			elem->data = ref;
			break;
		}
	}
}
/**
*@brief Creates and Allocates memory for the player. Then initializes all the
*@brief data members for Player
*/
void CreatePlayer(int x, int y)
{
 	playerEnt = CreateEntity();

	LoadPlayerAnimations(playerEnt);
	playerEnt->whatAmI = Aard;
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
	playerEnt->penalty = 0;
	for(int i =0;i < map->numberOfRooms;++i)
	{
		if(AABB(playerEnt->hitBox,map->rooms[i].boundary))
		{
			playerEnt->room = &map->rooms[i];
		}
	}

	playerEnt->update = &UpdatePlayer;
	playerEnt->draw = &DrawPlayer;
	playerEnt->think = &ThinkPlayer; 
	playerEnt->touch = &TouchPlayer;
	playerEnt->free = &FreePlayer;
	playerEnt->spiritIndex = 1;

 	SetElement(l,playerEnt);
	moveToPos(SpiritLink,1);
	Insert(SpiritLink,playerEnt);
	Next(SpiritLink);
	atexit(FreeInventory);
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
	if(playerData->confidence <= 0)
		printf("Player is Dead");
	ent->hitBox.x =ent->position.x;
	ent->hitBox.y =ent->position.y;
	
	Vec2DAdd(ent->position,ent->position,OverlapsMap(map,ent));
	Vec2DAdd(ent->position,ent->position,ent->velocity);
	EntityIntersectAll(ent);
	if(ent->atkBox.x > 0)
	{
		AttackBoxIntersectAll(ent);
	}
}

void ThinkPlayer(Entity *ent)
 {
	if(ent->stun > 0){
		Stun();
		playerEnt->stun -=.1;
	}
	ExecuteSkill();
	if(ent->nextThink < SDL_GetTicks())
	{
		for(int i = 0;i < 4;++i){
			if(playerData->abilities[i].cooldown > 0){
				playerData->abilities[i].cooldown--;
				playerData->abilities[i].inuse = 0;
			}
		}
		playerData->confidence += playerEnt->penalty;
		ent->nextThink = SDL_GetTicks() +1000;
	}
	playerData->guidingSpirits = SpiritLink->count;
	Vec2DAdd(ent->position,ent->position,OverlapsMap(map,ent));
	ent->atkBox.x = 0;
	ent->atkBox.y = 0;
	ent->atkBox.w = 0;
	ent->atkBox.h = 0;
	if(playerData->confidence <= 0)
	{
		FreePlayer(ent);
	}

	ent->room = 
		   ent->position.x > ent->room->boundary.x + ent->room->boundary.w ? ent->room = ent->room->west 
		:  ent->position.x < ent->room->boundary.x ? ent->room = ent->room->east
		:  ent->position.y > ent->room->boundary.y + ent->room->boundary.h ? ent->room = ent->room->south
		:  ent->position.y < ent->room->boundary.y ? ent->room = ent->room->north
		:  ent->room;
	
//Handle Animation Change
	ent->currentAnimation = keyData->ArrowKeyLeft != 0 || keyData->ArrowKeyRight != 0 ? 2:
		keyData->ArrowKeyUp == 1 ? 1:
		keyData->ArrowKeyDown == 1 ? 0:
		ent->currentAnimation;

	ent->sprite->animation[playerEnt->currentAnimation].holdFrame = ent
		->velocity.x == 0 && ent->velocity.y == 0 ? 1 : 0;
	ent->currentFrame = ent->sprite->animation[playerEnt->currentAnimation].currentFrame;
}
void TouchPlayer(Entity *ent,Entity *other)
{
	if(other->whatAmI == 6)
	{
		if(playerData->inventory->keys > 0)
		{
			(*other->free)(other);
			playerData->inventory->keys--;
		}
	}
	else if(other->whatAmI == 5)
	{
		LoadDungeon(other->dungeonName,other->savedPlayerPos);
	}
	else if((other->whatAmI == 4 || other->whatAmI == 8) && keyData->R == 1)
		SkillPickUpObject(other);
	if(ent->atkBox.w == 0 && ent->atkBox.h == 0)
	{
	if(other->whatAmI == 3)
	{
		moveToEnd(SpiritLink);
		for(int i = 1;i < SpiritLink->count;++i)
		{
			playerData->EXP += (SpiritLink->count-1) % 2 == 0 && SpiritLink->count != 1? 1 * (SpiritLink->count-1)/2: 0;
			playerData->confidence +=(SpiritLink->count-1) % 2 == 0 && SpiritLink->count != 1? 1 * (SpiritLink->count-1)/2: 0;
			FreeSpirit(SpiritLink->curr->curr);
			playerData->confidence = playerData->confidence < playerData->maxConfidence 
				? playerData->confidence+1 : playerData->maxConfidence;
			map->numOfSpirits--;
			playerData->rescuedSpirits++;
			playerData->EXP += 2;
		}

	}else if(other->whatAmI == 2)
	{
		//Move player back
		ent->velocity.x == 0 && ent->velocity.y == 0 ?
			Vec2DAdd(ent->position,ent->position,
				CreateVec2D(other->speed*6*other->velocity.x,other->speed*6*other->velocity.y)):
			Vec2DAdd(ent->position,ent->position,
				-CreateVec2D(ent->speed * ent->velocity.x ,ent->speed* ent->velocity.y));
			//Move Spirits back as well
			if(SpiritLink->count >=2){
			moveToPos(SpiritLink,2);
			for(int i = 1; i< SpiritLink->count;++i)
			{
					ent->velocity.x == 0 && ent->velocity.y == 0 ?
			Vec2DAdd(SpiritLink->curr->curr->position,SpiritLink->curr->curr->position,
				CreateVec2D(other->speed*6*other->velocity.x,other->speed*6*other->velocity.y)):
			Vec2DAdd(SpiritLink->curr->curr->position,SpiritLink->curr->curr->position,
				-CreateVec2D(ent->speed * ent->velocity.x ,ent->speed* ent->velocity.y));
			}
			}
		Vec2DAdd(other->position,other->position,-other->velocity);


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
void FreePlayer(Entity* ent)
{
	FreeEntity(ent);
	moveToPos(SpiritLink,1);
	if(SpiritLink->curr->curr == ent)
		Remove(SpiritLink);
	if(SpiritLink->count != 0)
		ClearSpiritLink();
	ent= NULL;

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
   SDL_SetRenderDrawColor(GetRenderer(), 0, 0, 0, 0); 
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
void SkillPickUpObject(Entity* ent)
{
	if(ent->whatAmI == 4){
		if(playerEnt->objectHolding == NULL && playerData->abilities[3].cooldown == 0)
		{
			playerEnt->objectHolding = ent;
			playerEnt->stun = 2;
			playerEnt->objectHolding->hitBox.w = playerEnt->objectHolding->hitBox.h = 0;
			playerEnt->objectHolding->hitBox.y = playerEnt->objectHolding->hitBox.x = 0;
			printf("Picked up object\n");
		}
	}else if(ent->whatAmI == 8)
		{
			ent->currentAnimation = 1;
			if(ent->itemHolding != NULL){
				if(ent->itemHolding->itemType == Key)
				{
					playerData->inventory->keys +=1;
				}
			}
		}
}

void SkillThrowObject()
{
		printf("threw the object\n");
		playerEnt->objectHolding->nextThink = SDL_GetTicks() + 800;
		playerEnt->objectHolding->temp = 1;
		if(playerEnt->facing.x != 0)
		{
			playerEnt->objectHolding->velocity.x = playerEnt->facing.x* playerEnt->objectHolding->speed;
			playerEnt->objectHolding->velocity.y = 2;
		}else if(playerEnt->facing.y != 0)
		{
		playerEnt->objectHolding->velocity.y = -playerEnt->facing.y*  playerEnt->objectHolding->speed;
		}
		playerEnt->objectHolding = NULL;
		playerData->abilities[3].inuse = 0;
		playerData->abilities[3].cooldown = 1;

}
void ExecuteSkill()
{
	if(keyData->Q == 1 && playerData->abilities[0].inuse == 0 
		&& playerData->abilities[0].cooldown == 0 && playerData->EXP >=2)
	{
		playerData->abilities[0].inuse = 1;
		playerData->abilities[0].cooldown = 3;
		printf("Whip used:%d\n" ,playerData->abilities[0].cooldown);
		SkillWhip();
	}
	else if(keyData->W == 1 && playerData->abilities[1].inuse == 0 
		&& playerData->abilities[1].cooldown == 0 && playerData->EXP >=4 && SpiritLink->count >1)
	{
		playerData->abilities[1].inuse = 1;
		playerData->abilities[1].cooldown = 3;
		printf("Retrieve used:%d\n" ,playerData->abilities[0].cooldown);
		SkillRetrieve();
	}
		else if(keyData->E == 1 && playerData->abilities[2].inuse == 0 
			&& playerData->abilities[2].cooldown == 0 && playerData->EXP >=6 && SpiritLink->count >1)
	{
		playerData->abilities[2].inuse = 1;
		playerData->abilities[2].cooldown = 10;
		printf("Immune used:%d\n" ,playerData->abilities[0].cooldown);
		SkillNEVERGONNAGIVEYOUUP();
	}
		else if(keyData->R == 1 && playerEnt->objectHolding != NULL && playerData->abilities[3].inuse == 0
			&& playerData->abilities[3].cooldown == 0)
		{
			playerData->abilities[3].inuse = 1;
			SkillThrowObject();
		}
		else if(keyData->Spacebar == 1)
		{
			UseItem();
		}
}
void UseItem()
{
	if(playerData->currentItem != NULL)
	{
		(*playerData->currentItem->useItem)(playerData->currentItem);
	}
	keyData->Spacebar = 0;
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
void Stun()
{
	if(playerEnt->stun > 0){
		playerEnt->velocity.x = 0;
		playerEnt->velocity.y = 0;
		keyData->Q = 0;
		keyData->W = 0;
		keyData->E = 0;
		keyData->R = 0;
	}
}
void FreeInventory()
{
	if (playerData->inventory != NULL)
	{
		g_list_foreach(playerData->inventory->inventory,GFunc(FreeItem),(ItemRef*)playerData->inventory->inventory->data);
		FreeSprite(playerData->inventory->sprite);
		FreeSprite(playerData->inventory->cursor->sprite);
		playerData->inventory->cursor = NULL;
		playerData->inventory = NULL;
		free(playerData->inventory);
	}
}