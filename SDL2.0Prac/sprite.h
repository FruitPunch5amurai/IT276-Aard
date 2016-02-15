
#ifndef _SPRITE_H_
#define _SPRITE_H_

typedef struct AnimationData{
	int currentFrame;
	int frameInc;
	int frameRate;
	long oldTime;
	int maxFrames;
	bool oscillate;

}Animation;
	void Animate(Animation* animation,int startFrame);
	void SetCurrentFrame(Animation* animation,int Frame);
	void SetFrameRate(Animation* animation);
	int GetCurrentFrame(Animation* animation);


typedef struct SpriteData{
  SDL_Texture *image;		/*pointer to the actual image in memory*/
  Animation* animation[20];	/*Array of animations for sprite*/
  char filename[30];			/*the name of the file, keep them short*/
  int w, h;					/*the width and height of the frames of the sprites, not the file itself*/
  int fpl;			/*default is 16*/
  int refCount;					/*used by the maintanenc functions to keep track of how many times a single sprite is being used*/
}Sprite;


void InitSpriteList();
void FreeSprite(Sprite *img);
Sprite *LoadSprite(char *filename,int sizex, int sizey);	
void DrawSprite(Sprite *sprite,int sx,int sy, int frame);
#endif