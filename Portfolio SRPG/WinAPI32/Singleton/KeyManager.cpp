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
    //이전에 눌렸건 안눌렸건 현재눌렸다면 1반환
    if (GetAsyncKeyState(KeyCode) & 0x8000) {
        if (!bKeyDown[KeyCode]) { //배열에 bool값이 false라면
            bKeyDown[KeyCode] = true;
            return true;
        }
    }
    //현재 눌리지 않았다면 false
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
