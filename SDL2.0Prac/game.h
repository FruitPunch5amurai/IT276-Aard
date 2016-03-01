#ifndef __GAME_H
#define __GAME_H


Vec2D GetCameraPosition();
Vec2D GetCameraSize();
Vec2D SetCameraPosition;
void SetCameraSize(Vec2D size);
SDL_Rect* GetCamera();	


void DrawTitle();
void DrawGame();
void Init();
void Loop();
void SetCamera(SDL_Rect &camera,Entity* ent);
int (*gameState)();
int Title();
int Game();

#endif
