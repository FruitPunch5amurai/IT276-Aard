
#ifndef __PARTICLE_H_
#define __PARTICLE_H_

typedef struct{
	int inuse;
	Vec2D position;
	int frame;
	Sprite* sprite;
	int maxFrames;

}Particle;
int isDead(Particle *p);
void DrawParticle(Particle *p,int animationNum,int x, int y);
Particle *CreateParticle(int x , int y,char filename[],int sizeX,int sizeY,int maxFrames);


#endif