#pragma once
#include "Singleton/Singleton.h"

class KeyManager: public  SingletonBase <KeyManager>
{
private:
    //0~255
    bool bKeyDown[256]; //각각의 키코드값의 true false
    BYTE PrevKey;       //전에 눌렀던 키코드

public:
    KeyManager();
    ~KeyManager();
    
    bool isKeyDown(int KeyCode); //눌렀을때
    bool StayKeyDown(int KeyCode);//누르고있을때
    bool isKeyUp(int KeyCode); //눌렀다가 떼었을때
};

