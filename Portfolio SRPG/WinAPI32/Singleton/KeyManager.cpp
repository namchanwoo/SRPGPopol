#include "stdafx.h"
#include "KeyManager.h"

KeyManager::KeyManager()
    :PrevKey(NULL)
{
    ZeroMemory(bKeyDown, sizeof(bKeyDown));
}

KeyManager::~KeyManager()
{
}

bool KeyManager::isKeyDown(int KeyCode)
{
    //������ ���Ȱ� �ȴ��Ȱ� ���紭�ȴٸ� 1��ȯ
    if (GetAsyncKeyState(KeyCode) & 0x8000) {
        if (!bKeyDown[KeyCode]) { //�迭�� bool���� false���
            bKeyDown[KeyCode] = true;
            return true;
        }
    }
    //���� ������ �ʾҴٸ� false
    else bKeyDown[KeyCode] = false;

    return false;
}

bool KeyManager::StayKeyDown(int KeyCode)
{
    if (GetAsyncKeyState(KeyCode) & 0x8000) {
        PrevKey = KeyCode;
        return true;
    }
    return false;
}

bool KeyManager::isKeyUp(int KeyCode)
{
    if (!StayKeyDown(KeyCode)) {
        if (PrevKey == KeyCode) {
            PrevKey = NULL;
            return true;
        }
    }
    return false;
}
