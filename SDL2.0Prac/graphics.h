
#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

void Init_Graphics(
	char *windowName,
    int viewWidth,
    int viewHeight,
    int renderWidth,
    int renderHeight,
    float bgcolor[4]);
void graphics_close();
void graphics_render_to_screen(SDL_Surface *surface,SDL_Rect srcRect,int x,int y);
void FrameDelay(Uint32 delay);
void ResetBuffer();
void NextFrame();
SDL_Renderer* getRenderer();


#endif