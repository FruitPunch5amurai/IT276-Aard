#ifndef __GAME_H
#define __GAME_H



typedef struct GameData{
	int (*gameState) ();
	SDL_Texture* mainSceneTexture;
	TTF_Font *font;
	SDL_Rect *camera;
}Game;

SDL_Rect* GetCamera();	
void DrawGame();
void DrawMainScene();
void Init();
void Loop();
void SetCamera(SDL_Rect &camera,SDL_Rect *box);
void UpdateHotBox();
SDL_Rect *InitHotBox();
void SetGameState(int (*state)());
void InitAll();


extern Game *game;
#endif
