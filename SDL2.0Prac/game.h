#ifndef __GAME_H
#define __GAME_H




	
	void DrawTitle();
	void DrawGame();
	void Init();
	void Loop();
	SDL_Rect* GetCamera();
	void SetCamera(SDL_Rect &camera,Player* player);
	int (*gameState)();
	int Title();
	int Game();

#endif
