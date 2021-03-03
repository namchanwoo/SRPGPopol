#pragma once
#include "Singleton/Singleton.h"
#define MAX_INPUT_KEY 255
//#define MAX_INPUT_MOUSE 8

class KeyManager: public  SingletonBase <KeyManager>
{
private:
	byte keyState[MAX_INPUT_KEY];
	byte keyOldState[MAX_INPUT_KEY];
	byte keyMap[MAX_INPUT_KEY];

	enum
	{
		KEY_INPUT_STATUS_NONE = 0,
		KEY_INPUT_STATUS_DOWN,
		KEY_INPUT_STATUS_UP,
		KEY_INPUT_STATUS_PRESS,
	};

public:
    KeyManager();
    ~KeyManager();
    
    bool isKeyDown(int KeyCode); //눌렀을때
    bool StayKeyDown(int KeyCode);//누르고있을때
    bool isKeyUp(int KeyCode); //눌렀다가 떼었을때
	void Update();
};

