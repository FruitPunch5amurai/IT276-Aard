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
static SDL_Renderer *GUIRenderer= NULL;
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
	buffer = SDL_GetWindowSurface(window);

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
	if(TTF_Init() == -1)
	{
		printf("Failed to init TTF");
		graphics_close();
        return;
	}

}
/**
*@brief Returns Renderer	
*/
SDL_Renderer* GetRenderer()
{
	return renderer;
}

SDL_Renderer* GetGUIRenderer()
{
	return GUIRenderer;
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
	if(TTF_WasInit)
	{
		TTF_Quit();
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
void RenderFont(char *message,SDL_Rect &rect,TTF_Font *f,SDL_Color *fg)
{
	SDL_Surface * text;
	SDL_Texture * texture;
	text = TTF_RenderText_Solid(f, message , *fg);
	texture = SDL_CreateTextureFromSurface(GetRenderer(),text);
	SDL_QueryTexture(texture,NULL,NULL,&rect.w,&rect.h);
	SDL_RenderCopy(GetRenderer(),texture,NULL,&rect);

}


