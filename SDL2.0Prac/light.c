#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "graphics.h"
#include "game.h"
#include "light.h"
extern SDL_Texture* LightBuffer;
GList *LightList = NULL;
extern Entity *playerEnt;
LightSource* CreateLight(Entity* source)
{
	LightSource* l;
	l = (LightSource*)malloc(sizeof(LightSource));
	memset(l,0,sizeof(LightSource));
	l->sprite = source->sprite2;
	l->source = source;
	l->src.x = 0;
	l->src.y = 0;
	l->src.w = source->sprite2->w;
	l->src.h = source->sprite2->h;
	LightList = g_list_append(LightList,l);
	atexit(CloseLightList);
	return l;
}
void DrawLights()
{
	GList *elem;
	LightSource*ref;
	SDL_Rect rect;
	int n,m;
	 m = g_list_length(LightList);
	if(g_list_length(LightList) != 0){
 		for(n = 0; n < g_list_length(LightList);++n)
		{
			elem = g_list_nth(LightList,n);
			ref = (LightSource*)elem->data;
			if(ref->source->inuse == 0)
			{
 				FreeLight(ref);
				LightList = g_list_delete_link(LightList,elem);
				continue;
			}
			if(ref->source->room != playerEnt->room)
			{
				continue;
			}
			rect.x = ref->source->position.x - ref->sprite->w/2 - game->camera->x;
			rect.y = ref->source->position.y -ref->sprite->h/2 - game->camera->y;
			
 			if(rect.x + game->camera->x <= game->camera->x)
			{
				rect.x =0; 
			}
			if(rect.y + game->camera->y <= game->camera->y)
			{
				rect.y =0; 
			}
			rect.w = ref->sprite->w;
			rect.h = ref->sprite->h;
		    SDL_SetRenderTarget(GetRenderer(),LightBuffer);
		    SDL_RenderClear(GetRenderer());
		    SDL_RenderCopy(GetRenderer(),ref->sprite->image,&ref->src,&rect);
		    SDL_SetTextureBlendMode(game->mainSceneTexture,SDL_BLENDMODE_ADD); 
		    SDL_RenderCopy(GetRenderer(),game->mainSceneTexture,&rect,&rect);
		    SDL_SetRenderTarget(GetRenderer(),NULL);	
		    SDL_RenderCopy(GetRenderer(),LightBuffer,&rect,&rect);	
		}

	}
}
void FreeLight(LightSource* l)
{
	FreeSprite(l->sprite);
	free(l);
	l= NULL;
}
void CloseLightList()
{
	GList *elem;
	LightSource* ref;
	int n;
		if(g_list_length(LightList) != 0){
			for(n = 0; n < g_list_length(LightList);++n)
			{
				elem = g_list_nth(LightList,n);
				ref = (LightSource*)elem->data;
				LightList = g_list_remove(LightList,ref);
				FreeLight(ref);
			}
		}
}
void InitLightList()
{
	LightList = NULL;
}