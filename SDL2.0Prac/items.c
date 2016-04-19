#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <jansson.h>
#include <glib.h>
#include "sprite.h"
#include "graphics.h"
#include "items.h"
extern Entity* playerEnt;
extern Map* map;
extern SDL_Texture* mainSceneTexture;
extern SDL_Rect *camera;
extern TTF_Font *globalFont;
ItemRef* LoadItem(ItemType item)
{
	//json_t *root;
	//json_error_t err;
	int ItemType;
	char* filepath;
	Items *i;
	ItemRef* ref;
	i = (Items*)malloc(sizeof(Items));
	memset(i,0,sizeof(Items));	
	ref = (ItemRef*)malloc(sizeof(ItemRef));
	memset(ref,0,sizeof(ItemRef));
	i->itemType = item;
	i->isEquiped = 0;
	/*
	root = json_load_file("Items.json",0,&err);
	if(!root)
	{
		fprintf(stderr,"error: on line %d: %s\n", err.line,err.text);
		exit(1);
	}
	json_t *data,*arr;
	data = json_object_get(root,"Items");
	arr = json_array_get(data,0);
	json_unpack(arr,"{s:i,s:s}","ItemType",&ItemType,"filepath",
		&filepath);
	*/
	if(item == 0)
	{
	i->itemType = Lantern;
	i->useItem = &ItemLantern;
	ref->itemType = Lantern;
	//i->sprite = LoadSprite(filepath,16,16);
	i->itemID = -1;
	i->self = NULL;
	i->self = CreateEntity();
	i->self->whatAmI = Light;
	
	i->self->sprite = LoadSprite("images/lightsource1.png",255,255);
	i->self->sprite->animation[0].currentFrame = 0;
	i->self->sprite->animation[0].maxFrames = 0;
	i->self->sprite->animation[0].startFrame = 0;
	
	i->self->currentAnimation = 0;
	i->isEquiped = 1;
	i->self->dimensions.x = 96;
	i->self->dimensions.y = 96;

	ref->item = i;
	ref->sprite = LoadSprite("images/LanternRef.png",45,45);
	}
	else if(item == 1)
	{
		i->itemType = Key;
		ref->itemType = Key;
		ref->item = i;
		ref->sprite = LoadSprite("images/Key.png",16,32);
	}
	//json_decref(root);
	ItemType = NULL;
	filepath = NULL;
	i = NULL;
	return ref;
	
}
void UseItem(Items *item)
{
	item->useItem;
}
void ItemLantern(Items *item)
{
	if(item->itemType != Lantern)
	{
		printf("Item is not a lantern\n");
		return;
	}
 	if(item->isEquiped == 0){
		if(item->self == NULL)
			item->self = LoadItem(Lantern)->item->self;
 		item->isEquiped = 1;
		item->self->update = &UpdateLantern;
		item->self->draw = &DrawLantern;
		item->self->position.x = playerEnt->position.x + 50;
		item->self->position.y = playerEnt->position.y + 50;
		printf("Used Lantern!\n");
	}else
	{
		item->isEquiped = 0;
 		FreeEntity(item->self);
		item->self = NULL;
		printf("Unused Lantern\n");
	}
}
void UpdateLantern(Entity* ent)
{
	ent->savedPlayerPos.x = playerEnt->position.x;
	ent->savedPlayerPos.y = playerEnt->position.y;
	ent->position.x =ent->savedPlayerPos.x - ent->sprite->w/2;
	ent->position.y =ent->savedPlayerPos.y - ent->sprite->h/2;
}
void DrawLantern(Entity* ent)
{
	SDL_Rect dest;

	dest.x = (ent->savedPlayerPos.x - ent->sprite->w/2) - camera->x;
	dest.y = (ent->savedPlayerPos.y - ent->sprite->h/2) - camera->y;
	dest.w = ent->sprite->w;
	dest.h = ent->sprite->h;
 	SDL_SetRenderTarget(GetRenderer(),ent->sprite->image);
	SDL_SetTextureBlendMode(ent->sprite->image,SDL_BLENDMODE_ADD); 
	SDL_RenderCopy(GetRenderer(),mainSceneTexture,&dest,&dest);
	SDL_SetRenderTarget(GetRenderer(),NULL);	
	DrawEntity(ent,0,ent->position.x,ent->position.y);
}
Inventory* InitInventory()
{
	Inventory* i;
	int k;
	int x = 132, y = 132;
	ItemRef* ref;
	InventoryCursor* c;
	c = (InventoryCursor*)malloc(sizeof(InventoryCursor));
	c->sprite = LoadSprite("images/InventoryCursor.png",45,45);
	c->ref = NULL;
	i = (Inventory*)malloc(sizeof(Inventory));
	i->inventory = NULL;
	i->display = 0;
	i->sprite = LoadSprite("images/InventoryBackground.png",319,250);
	i->cursor = c;
	i->cursor->ref = g_list_first(i->inventory);
	for(k = 0;k < 12;++k)
	{
		ref = (ItemRef*)malloc(sizeof(ItemRef));
		ref->item = NULL;
		ref->pos.x = x;
		ref->pos.y = y;
		if(x > 132 + 45 *4)
		{
			y += 45;
			x= 132;
		}else
		{
			ref->pos.y = y;
			x += 45;
		}
		i->inventory = g_list_append(i->inventory,ref);

	}
	return i;
}
void DrawInventory(Inventory* i)
{
	SDL_Rect src,dst;
	src.w = i->sprite->w;
	src.h = i->sprite->h;
	src.x = 0;
	src.y = 0;
	dst.w = i->sprite->w;
	dst.h = i->sprite->h;
	dst.x = 100;
	dst.y = 100;
	SDL_RenderCopy(GetRenderer(),i->sprite->image,&src,&dst);
	DrawCursor(i);
	DrawItemRef(i);
}
void DrawCursor(Inventory* i)
{
	ItemRef* ref;
	SDL_Rect src,dst;
	ref= (ItemRef*)i->cursor->ref->data;
	src.w = i->cursor->sprite->w;
	src.h = i->cursor->sprite->h;
	src.x = 0;
	src.y = 0;
	dst.w = i->cursor->sprite->w;
	dst.h = i->cursor->sprite->h;
	dst.x = ref->pos.x;
	dst.y = ref->pos.y;
	SDL_RenderCopy(GetRenderer(),i->cursor->sprite->image,&src,&dst);
}
void DrawItemRef(Inventory* i)
{
	SDL_Rect src,dst;
	GList * elem;
	ItemRef* ref;
	int n;
	for(n = 0; n < g_list_length(i->inventory);++n)
	{
		elem = g_list_nth(i->inventory,n);
		ref = (ItemRef*)elem->data;
		if(ref->item != NULL){
		src.x = 0;
		src.y = 0;
		src.w = ref->sprite->w;
		src.h = ref->sprite->h;
		dst.x = ref->pos.x;
		dst.y = ref->pos.y;
		dst.w = src.w;
		dst.h = src.h;
		SDL_RenderCopy(GetRenderer(),ref->sprite->image,&src,&dst);
		}
	}
	ref = NULL;
	elem = NULL;
}
void AddItemToInventory(ItemRef* item)
{


}

void FreeItem(ItemRef *item)
{
	if(item->item != NULL){
		FreeEntity(item->item->self);
		FreeSprite(item->sprite);
		item->item->self = NULL;
		item->item = NULL;
	}
	item = NULL;
}