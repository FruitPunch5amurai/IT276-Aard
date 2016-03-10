#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "entity.h"
#include "collision.h"


SDL_Rect rect (int x, int y, int w, int h)
{
	SDL_Rect r;
	r.x = x;
	r.y = y;
	r.w = w;
	r.h = h;
	return r;
}

int AABB(SDL_Rect ent1,SDL_Rect ent2)
{
	if((ent1.x + ent1.w > ent2.x) &&
		(ent2.x + ent2.w > ent1.x) &&
		(ent1.y +ent1.h >ent2.y) &&
		(ent2.y + ent2.h >= ent1.y))
		return 1;
	else	
		return 0;


}
