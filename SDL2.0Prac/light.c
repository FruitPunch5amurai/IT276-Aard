#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "graphics.h"
#include "game.h"
#include "light.h"
extern SDL_Texture* LightBuffer;
GList *LightList = NULL;
LightSource* CreateLight(Entity* source)
{
	LightSource* l;
	l = (LightSource*)malloc(sizeof(LightSource));
	memset(l,0,sizeof(LightSource));
	l->sprite = source->sprite2;
	l->source = source;
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
			rect.x = ref->source->position.x - ref->sprite->w/2 - game->camera->x;
			rect.y = ref->source->position.y -ref->sprite->h/2 - game->camera->y;
			rect.w = ref->sprite->w;
			rect.h = ref->sprite->h;
		    SDL_SetRenderTarget(GetRenderer(),LightBuffer);
		    SDL_RenderClear(GetRenderer());
		    SDL_RenderCopy(GetRenderer(),ref->sprite->image,NULL,&rect);
		    SDL_SetTextureBlendMode(game->mainSceneTexture,SDL_BLENDMODE_ADD); 
		    SDL_RenderCopy(GetRenderer(),game->mainSceneTexture,&rect,&rect);
		    SDL_SetRenderTarget(GetRenderer(),NULL);	
		    SDL_RenderCopy(GetRenderer(),LightBuffer,&rect,&rect);	
		}

	}
}
void FreeLight(LightSource* l)
{
	free(l);
	l= NULL;
}
void CloseLightList()
{
	g_list_free(LightList);
}
void InitLightList()
{
	LightList = NULL;
}