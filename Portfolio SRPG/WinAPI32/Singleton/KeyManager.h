#pragma once
#include "Singleton/Singleton.h"

class KeyManager: public  SingletonBase <KeyManager>
{
private:
    //0~255
    bool bKeyDown[256]; //������ Ű�ڵ尪�� true false
    BYTE PrevKey;       //���� ������ Ű�ڵ�

public:
    KeyManager();
    ~KeyManager();
    
    bool isKeyDown(int KeyCode); //��������
    bool StayKeyDown(int KeyCode);//������������
    bool isKeyUp(int KeyCode); //�����ٰ� ��������
};

