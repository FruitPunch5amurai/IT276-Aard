#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <time.h>
#include <math.h>
#include <string>
#include "graphics.h"
#include "sprite.h"
#include "vectors.h"
#include "particle.h"
int isDead(Particle *p)
{
	return p->frame > p->maxFrames;
}
void DrawParticle(Particle *p,int animationNum,int x, int y)
{
		Animate(&p->sprite->animation[animationNum],p->sprite->animation[animationNum].startFrame);
	DrawSprite(p->sprite,x
		,y,GetCurrentFrame(&p->sprite->animation[animationNum]),
		GetRenderer(),SDL_FLIP_NONE);
	p->frame++;
	p->position.y -=.5;
}
Particle *CreateParticle(int x , int y,char filename[],int sizeX,int sizeY,int maxFrames)
{
	Particle *p;
	p = (Particle*)malloc(sizeof(Particle));
	p->sprite = LoadSprite(filename,sizeX,sizeY);
	p->frame = rand() % 20;
	p->maxFrames = maxFrames;
	p->sprite->animation[0].currentFrame = 0;
	p->sprite->animation[0].startFrame = 0;
	p->sprite->animation[0].maxFrames = 2;
	p->sprite->animation[0].oscillate = 1;
	p->sprite->animation[0].frameRate = 700;
	p->position.x = x - 5 + ( rand() % 25 );
    p->position.y = y - 5 + ( rand() % 25 );
	return p;
}


