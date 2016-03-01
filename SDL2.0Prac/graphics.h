
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
void FrameDelay();
void ResetBuffer();
void NextFrame();

void FillRect(int x, int y, int width, int height, int r, int g, int b);
void DrawRect(int x, int y, int width, int height, int r, int g, int b);
Uint32 GetSystemTime();
SDL_Renderer* GetRenderer();


#endif