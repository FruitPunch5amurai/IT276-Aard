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
	Uint8 Spacebar;
	Uint8 Enter;
	Uint8 BackSpace;
	Uint8 MouseButtonUp;

}KeyData;
void InitKeyData();
void CloseKeyData();
int StateTitle();
int StateGame();
int StateInventory();
int StateEditor();
void handleInput(int (*gameState)());
KeyData* GetKeyData();

#endif