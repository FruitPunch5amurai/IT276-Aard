#ifndef __GAME_H
#define __GAME_H





	void DrawTitle();
	void DrawGame();
	void Init();
	void Loop();

	int (*gameState)();
	int Title();
	int Game();

#endif
