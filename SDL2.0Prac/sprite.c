#include <math.h>
#include <string>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "graphics.h"
#include "sprite.h"

#define MAX_SPRITES 255
extern SDL_Renderer *renderer;
Sprite SpriteList[MAX_SPRITES];
int Num_Sprites;
void InitSpriteList()
{
	int x,j;
	memset(SpriteList,0,sizeof(Sprite) * MAX_SPRITES);
	for(x = 0;x < MAX_SPRITES;x++){
		for(j = 0;j < 20;j++)
	{
		SpriteList[x].animation[j] = (Animation*) malloc(sizeof(Animation));
		SpriteList[x].animation[j] ->currentFrame = 0;
		SpriteList[x].animation[j]->maxFrames = 0;
		SpriteList[x].animation[j]->frameInc = 1;
		SpriteList[x].animation[j]->frameRate = 100;
		SpriteList[x].animation[j]->oldTime = 0;
		SpriteList[x].animation[j]->oscillate = false;
	}
	SpriteList[x].image = NULL;
	}
}
void FreeSprite(Sprite *sprite)
{
  /*first lets check to see if the sprite is still being used.*/
  sprite->refCount--;
  if(sprite->refCount == 0)
  {
  strcpy(sprite->filename,"\0");
     /*just to be anal retentive, check to see if the image is already freed*/
  if(sprite->image != NULL)SDL_DestroyTexture(sprite->image);
  sprite->image = NULL;
  }
}

Sprite *LoadSprite(char *filename,int sizex, int sizey)
{
	
	int i;
	SDL_Surface *temp;
	SDL_Texture* temp2;
	for(i = 0; i<Num_Sprites;i++)
	{
		if(strncmp(filename,SpriteList[i].filename,30) == 0)
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
  temp2 = SDL_CreateTextureFromSurface(renderer,temp);
  if(temp2 == NULL)
  {
	  fprintf(stderr,"CREATE TEXTURE FROM SURFACE HAS FAILED: %s\n",SDL_GetError());
	  exit(0);
  }
  SDL_FreeSurface(temp);
  SpriteList[i].image = temp2;
// SDL_SetColorKey(SpriteList[i].image, SDL_TRUE , SDL_MapRGB(SpriteList[i].image->format, 255,255,255));
   /*then copy the given information to the sprite*/
  strncpy(SpriteList[i].filename,filename,20);
      /*now sprites don't have to be 16 frames per line, but most will be.*/
  SpriteList[i].fpl= 16;
  SpriteList[i].w = sizex;
  SpriteList[i].h = sizey;
  SpriteList[i].refCount++;
  return &SpriteList[i];
}
void DrawSprite(Sprite *sprite,int sx,int sy, int frame)
{
	SDL_Rect dest,src;
	src.x = frame%sprite->fpl * sprite->w;
    src.y = frame/sprite->fpl * sprite->h;
    src.w =sprite->w;
    src.h =sprite->h;
	dest.x = sx;
    dest.y = sy;
    dest.w = sprite->w;
    dest.h = sprite->h;
	SDL_RenderCopy(renderer, sprite->image, &src,&dest);
	
}
 
void Animate(Animation* animation,int startFrame) {
    if(animation->oldTime + animation->frameRate > SDL_GetTicks()) {
        return;
    }
 
    animation->oldTime = SDL_GetTicks();
 
    animation->currentFrame += animation->frameInc;
 
    if(animation->oscillate) {
        if(animation->frameInc > 0) {
			if(animation->currentFrame >= animation->maxFrames + startFrame) 
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
        if(animation->currentFrame >= animation->maxFrames) {
            animation->currentFrame = 0;
        }
    }
}
 
void SetFrameRate(Animation* animation,int Rate) {
    animation->frameRate = Rate;
}
 
void SetCurrentFrame(Animation* animation,int Frame) {
    if(Frame < 0 || Frame >= animation->maxFrames) return;
 
    animation->currentFrame = Frame;
}
 
int GetCurrentFrame(Animation* animation) {
    return animation->currentFrame;
}


