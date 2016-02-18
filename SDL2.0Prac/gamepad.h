#ifndef _GAMEPAD_H_
#define _GAMEPAD_H_


typedef struct{
	Uint8 ArrowKeyRight;
	Uint8 ArrowKeyLeft;
	Uint8 ArrowKeyUp;
	Uint8 ArrowKeyDown;
}ArrowKeyData;

void handleInput(int (*gameState)());

#endif