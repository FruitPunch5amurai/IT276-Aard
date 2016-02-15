#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <math.h>
#include <string>
#include <stdlib.h>
#include "graphics.h"


static SDL_Window* window = NULL;
SDL_Renderer *renderer = NULL;
static SDL_Texture  * texture = NULL;
static SDL_Surface  * surface = NULL;
static SDL_Surface  * temp_buffer = NULL;

SDL_Surface *buffer; /*pointer to the background image buffer*/
SDL_Surface *videobuffer; /*pointer to the draw buffer*/
SDL_Rect Camera; /*x & y are the coordinates for the background map, w and h are of the screen*/

Uint32 NOW;		

static int bitdepth;
static Uint32 rmask;
static Uint32 gmask;
static Uint32 bmask;
static Uint32 amask;

void Init_Graphics(
	char *windowName,
    int viewWidth,
    int viewHeight,
    int renderWidth,
    int renderHeight,
    float bgcolor[4])
{
	SDL_Rect grass_rect;
	bool quit = false;
	

	if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("Unable to initilaize SDL sys: %s",SDL_GetError());
		graphics_close();
		return;
	}else{printf("SDL Init Success\n");}
	
	window = SDL_CreateWindow(windowName,SDL_WINDOWPOS_UNDEFINED,
								SDL_WINDOWPOS_UNDEFINED,
								renderWidth, renderHeight,
								SDL_WINDOW_SHOWN);
	if(!window)
	{
		fprintf(stdout,"Window creation failed: %s",SDL_GetError());
		graphics_close();
		return;
	}

	renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED
											| SDL_RENDERER_TARGETTEXTURE);
	if (!renderer)
    {
        printf("failed to create renderer: %s",SDL_GetError());
		graphics_close();
        return;
    }else{
		printf("SDL Renderer Success\n");
	}
	/*
	texture = IMG_LoadTexture(renderer,"images/grass.png");
	if(texture == NULL)
	{
		printf("Error: Texture is NULL");
		return;
	}
	grass_rect.x = 0;
	grass_rect.y = 0;
	grass_rect.w = 800;
	grass_rect.h = 600;
    printf("graphics initialized\n");
	*/
}
SDL_Renderer* getRenderer()
{
	return renderer;
}

void NextFrame()
{
  Uint32 Then;
  SDL_RenderPresent(renderer);
  Then = NOW;									/*these next few lines  are refCount to show how long each frame takes to update.  */
  NOW = SDL_GetTicks();
/*  fprintf(stdout,"Ticks passed this frame: %i\n", NOW - Then);*/
  FrameDelay(33); /*this will make your frame rate about 30 frames per second.  If you want 60 fps then set it to about 15 or 16*/
}
void FrameDelay(Uint32 delay)
{
    static Uint32 pass = 100;
    Uint32 dif;
    dif = SDL_GetTicks() - pass;
    if(dif < delay)SDL_Delay( delay - dif);
    pass = SDL_GetTicks();
}
/*
void graphics_render_to_screen(SDL_Surface *surface,SDL_Rect srcRect,int x,int y)
{
	SDL_Rect dstRect;
    SDL_Point point = {1,1};
    int w,h;
    if (!texture)
    {
        printf("gt_graphics_render_surface_to_screen: no texture available");
        return;
    }
    if (!surface)
    {
        printf("gt_graphics_render_surface_to_screen: no surface provided");
        return;
    }
    SDL_QueryTexture(texture,
                     NULL,
                     NULL,
                     &w,
                     &h);
    /*check if resize is needed
    if ((surface->w > w)||(surface->h > h))
    {
        SDL_DestroyTexture(texture);
        texture = SDL_CreateTexture(renderer,
                                                   surface->format->format,
                                                   SDL_TEXTUREACCESS_STREAMING, 
                                                   surface->w,
                                                   surface->h);
        if (!texture)
        {
            printf("render_to_screen: failed to allocate more space for the screen texture!");
            return;
        }
    }
    SDL_SetTextureBlendMode(texture,SDL_BLENDMODE_BLEND);        
    SDL_UpdateTexture(texture,
                      &srcRect,
                      surface->pixels,
                      surface->pitch);
    dstRect.x = x;
    dstRect.y = y;
    dstRect.w = srcRect.w;
    dstRect.h = srcRect.h;
    SDL_RenderCopy(renderer,
                   texture,
                     &srcRect,
                     &dstRect);
}
*/
void graphics_close()
{
    if (texture)
    {
        SDL_DestroyTexture(texture);
    }
    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
    }
    if (window)
    {
        SDL_DestroyWindow(window);
    }
    if (surface)
    {
        SDL_FreeSurface(surface);
    }
    if (temp_buffer)
    {
        SDL_FreeSurface(temp_buffer);
    }
    surface = NULL;
    window = NULL;
    renderer = NULL;
    texture = NULL;
    temp_buffer = NULL;
}
