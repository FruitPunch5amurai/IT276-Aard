
#ifndef _SPRITE_H_
#define _SPRITE_H_


#define MAX_SPRITES 255
#define MAX_ANIMATIONS 20

typedef struct AnimationData{
	int startFrame;
	int currentFrame;
	int frameInc;
	int frameRate;
	long oldTime;
	int maxFrames;
	int oscillate;

}Animation;
	void Animate(Animation* animation,int startFrame);
	void SetCurrentFrame(Animation* animation,int Frame);
	void SetFrameRate(Animation* animation);
	int GetCurrentFrame(Animation* animation);


typedef struct SpriteData{
  SDL_Surface *surfaceImage;	
  SDL_Texture *image;		/*pointer to the actual image in memory*/
  Animation* animation[MAX_ANIMATIONS];	/*Array of animations for sprite*/
  char filename[30];			/*the name of the file, keep them short*/
  int w, h;					/*the width and height of the frames of the sprites, not the file itself*/
  int fpl;			/*default is 16*/
  int refCount;					/*used by the maintanenc functions to keep track of how many times a single sprite is being used*/
}Sprite;

void InitSpriteList();
void FreeSprite(Sprite *img);
void FreeAnimations(Sprite *sprite);
Sprite *LoadSprite(char *filename,int sizex, int sizey);	
void DrawSprite(Sprite *sprite,int sx,int sy, int frame,SDL_Renderer *renderer,SDL_RendererFlip flip);
void SetColor(Sprite *sprite,Uint8 red, Uint8 green, Uint8 blue);
void PlaySFX(char *filename,int sizex,int sizey,int x, int y);
#endif