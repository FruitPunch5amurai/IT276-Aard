#ifndef _GAMEPAD_H_
#define _GAMEPAD_H_


typedef struct{
	Uint8 ArrowKeyRight;
	Uint8 ArrowKeyLeft;
	Uint8 ArrowKeyUp;
	Uint8 ArrowKeyDown;
	Uint8 Q;
	Uint8 W;
	Uint8 E;
	Uint8 R;

}KeyData;
void InitKeyData();
void CloseKeyData();
void handleInput(int (*gameState)());

#endif