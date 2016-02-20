#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "vectors.h"
#include <math.h>
#include <string>
#include <stdlib.h>
#include "graphics.h"


static SDL_Window* window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture  * texture = NULL;
static SDL_Surface  * surface = NULL;
static SDL_Surface  * temp_buffer = NULL;


SDL_Surface *buffer; /*pointer to the background image buffer*/
SDL_Surface *videobuffer; /*pointer to the draw buffer*/
SDL_Rect Camera; /*x & y are the coordinates for the background map, w and h are of the screen*/

/*timing*/
static Uint32 Delay = 45;
static Uint32 NOW = 0;
static Uint32 THEN = 0;
static Uint8 PrintFps = false;
static Uint32 FPS = 0; 

static int bitdepth;
static Uint32 rmask;
static Uint32 gmask;
static Uint32 bmask;
static Uint32 amask;
/**
*@brief Initialized Main window
*/
void Init_Graphics(
	char *windowName,
    int viewWidth,
    int viewHeight,
    int renderWidth,
    int renderHeight,
    float bgcolor[4])
{
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
/**
*@brief Returns Renderer	
*/
SDL_Renderer* GetRenderer()
{
	return renderer;
}

void FrameDelay()
{
    Uint32 diff;
    THEN = NOW;
    NOW = SDL_GetTicks();
    diff = (NOW - THEN);
    if (diff < Delay)
    {
        SDL_Delay(Delay - diff);
    }
    FPS = 1000.0/MAX(SDL_GetTicks() - THEN,0.001);
}

void NextFrame()
{
	SDL_RenderPresent(renderer);
    FrameDelay();
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
/**
*@brief Closes all windows and textures
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
Uint32 GetSystemTime()
{
	return NOW;
}