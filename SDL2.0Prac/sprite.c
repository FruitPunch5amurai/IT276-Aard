#include <math.h>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "graphics.h"
#include "game.h"
#include "sprite.h"

//Stack is were all the core parts of a program go to

static Sprite *SpriteList = NULL;
int Num_Sprites;
/**
*@brief Initializes Sprite List
*/
void InitSpriteList()
{
	int x,j;
	SpriteList = (Sprite*)malloc(sizeof(Sprite) * MAX_SPRITES);
	memset(SpriteList,0,sizeof(Sprite) * MAX_SPRITES);
	for(x = 0;x < MAX_SPRITES;x++){
		for(j = 0;j < 20;j++)
	{
		SpriteList[x].animation[j].currentFrame = 0;
		SpriteList[x].animation[j].maxFrames = 0;
		SpriteList[x].animation[j].frameInc = 1;
		SpriteList[x].animation[j].holdFrame = 0;
		SpriteList[x].animation[j].frameRate = 100;
		SpriteList[x].animation[j].oldTime = 0;
		SpriteList[x].animation[j].oscillate = false;
	}
	SpriteList[x].image = NULL;
	}
	atexit(CloseSpriteSystem);
}
/**
*@brief Deallocates memory used for sprites
*/
void CloseSpriteSystem()
{
	int i,j;
	if(!SpriteList)
	{
		return;
	}
	for (i = 0;i < MAX_SPRITES;i++)
	{
		if(SpriteList[i].image != 0)
		{
			FreeSprite(&SpriteList[i]);
		}
		
	}
	free(SpriteList);
	SpriteList = NULL;
}

/**
*@brief Deallocates and frees up a sprite
*/
void FreeSprite(Sprite *sprite)
{
	int i;
	if(sprite == NULL)
	{
		return;
	}
  /*first lets check to see if the sprite is still being used.*/
  if(sprite->refCount > 0)
  {
	sprite->refCount--;
  }
if(sprite->refCount <= 0)
  {
  strcpy(sprite->filename,"\0");
     /*just to be anal retentive, check to see if the image is already freed*/
  if(sprite->image != NULL)
	  SDL_DestroyTexture(sprite->image);
  sprite->image = NULL;
  }
	for(i = 0; i < MAX_ANIMATIONS;++i)
	{
		sprite->animation[i].currentFrame = 0;
		sprite->animation[i].maxFrames = 0;
		sprite->animation[i].frameInc = 1;
		sprite->animation[i].holdFrame = 0;
		sprite->animation[i].frameRate = 100;
		sprite->animation[i].oldTime = 0;
		sprite->animation[i].oscillate = false;
	}
  sprite = NULL;
}

/**
*@brief Load in data for a sprite
*/
Sprite *LoadSprite(char *filename,int sizex, int sizey)
{
	
	int i;
	SDL_Surface *temp;
	SDL_Texture* temp2;
	for(i = 0; i<Num_Sprites;i++)
	{
		if(strncmp(filename,SpriteList[i].filename,40) == 0 && SpriteList[i].w == sizex && SpriteList[i].h == sizey)
		{
			SpriteList[i].refCount++;
			return &SpriteList[i];
		}
	}
	if(Num_Sprites + 1 >= MAX_SPRITES)
	{
		fprintf(stderr,"Max Sprites Reached.\n");
		exit(1);
	}
	Num_Sprites++;
  for(i = 0;i <= Num_Sprites;i++)
  {
    if(!SpriteList[i].refCount)break;
  }
   temp = IMG_Load(filename);
  if(temp == NULL)
  {
    fprintf(stderr,"unable to load a vital sprite: %s\n",SDL_GetError());
    exit(0);
  }
  temp2 = SDL_CreateTextureFromSurface(GetRenderer(),temp);
  if(temp2 == NULL)
  {
	  fprintf(stderr,"CREATE TEXTURE FROM SURFACE HAS FAILED: %s\n",SDL_GetError());
	  exit(0);
  }
  SpriteList[i].surfaceImage = temp;
  SpriteList[i].image = temp2;
// SDL_SetColorKey(SpriteList[i].image, SDL_TRUE , SDL_MapRGB(SpriteList[i].image->format, 255,255,255));
   /*then copy the given information to the sprite*/
  strncpy(SpriteList[i].filename,filename,30);
      /*now sprites don't have to be 16 frames per line, but most will be.*/
  SpriteList[i].fpl= 16;
  SpriteList[i].w = sizex;
  SpriteList[i].h = sizey;
  SpriteList[i].refCount++;
  return &SpriteList[i];
}
/**
*@brief Draws specified sprite
*/
void DrawSprite(Sprite *sprite,int sx,int sy, int frame,SDL_Renderer *renderer,SDL_RendererFlip flip)
{
	SDL_Rect dest,src;
	src.x = frame%sprite->fpl * sprite->w;
    src.y = frame/sprite->fpl * sprite->h;
    src.w =sprite->w;
    src.h =sprite->h;
	dest.x = sx - game->camera->x;
    dest.y = sy - game->camera->y;
    dest.w = sprite->w;
    dest.h = sprite->h;
	SDL_RenderCopyEx(renderer, sprite->image,&src,&dest,0,0,flip);

	
}


 /**
*@brief Increases or decrease the current frame of the animation
*/
void Animate(Animation* animation,int startFrame) {
    if(animation->oldTime + animation->frameRate > SDL_GetTicks()) {
        return;
    }
 
    animation->oldTime = SDL_GetTicks();
	if(animation->holdFrame == 1)
	{
		animation->currentFrame = animation->startFrame + animation->heldFrame - 1;
		return;
	}
    animation->currentFrame += animation->frameInc;
 
    if(animation->oscillate) {
        if(animation->frameInc > 0) {
			if(animation->currentFrame >= animation->maxFrames-1 + startFrame) 
			{
                animation->frameInc = -animation->frameInc;
            }
        }else{
            if(animation->currentFrame <= startFrame) 
			{
                animation->frameInc = -animation->frameInc;
            }
        }
    }else{
        if(animation->currentFrame >= animation->maxFrames + startFrame) {
            animation->currentFrame = startFrame;
        }
    }
}
/**
*@brief Set framerate of specified animation
*/
void SetFrameRate(Animation* animation,int Rate) {
    animation->frameRate = Rate;
}
/**
*@brief Set current frame of animation
*/
void SetCurrentFrame(Animation* animation,int Frame) {
    if(Frame < 0 || Frame >= animation->maxFrames) return;
 
    animation->currentFrame = Frame;
}
/**
*@brief Returns current frame of specified animation
*/
int GetCurrentFrame(Animation* animation) {
    return animation->currentFrame;
}

void SetColor(Sprite *sprite,Uint8 red, Uint8 green, Uint8 blue)
{
	SDL_SetTextureColorMod(sprite->image,red,green,blue);

}
Sprite* GetSpriteList()
{
	return SpriteList;
}



