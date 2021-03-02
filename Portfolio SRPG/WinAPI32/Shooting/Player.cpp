#include "stdafx.h"
#include "Player.h"

HRESULT Player::init()
{
    img = new Image();
    img->init(_T("walk.bmp"));
    img->Scale = Vector2(50.0f,50.0f);
    img->MaxFrameX = 6;
    img->MaxFrameY = 8;
    pos = Vector2(100.0f,200.0f);

    PlDir = DirN;
    Seta45 = PI/4.0f;
    return S_OK;
}

void Player::release()
{
}

void Player::update()
{
    //�¿� ����Ű �Ұ�
    PlDir = DirN;
    if(KEYMANAGER->StayKeyDown(VK_LEFT))
    {
        PlDir = DirL;
    }
    else if(KEYMANAGER->StayKeyDown(VK_RIGHT))
    {
        PlDir = DirR;
    }
    if(KEYMANAGER->StayKeyDown(VK_UP))
    {
        if(PlDir != DirN)
        {
            (PlDir == DirL)? PlDir = DirLU:PlDir = DirRU;
        }
        else
        {
            PlDir = DirU;
        }
    }
    else if(KEYMANAGER->StayKeyDown(VK_DOWN))
    {
        if(PlDir != DirN)
        {
            (PlDir == DirL)? PlDir = DirLD:PlDir = DirRD;
        }
        else
        {
            PlDir = DirD;
        }
    }

    if(PlDir != DirN)
    {
        img->CurrentFrameY = PlDir;
    }
    switch(PlDir)
    {
       case DirR:
           dir = RIGHT;
        break;
       case DirU:
           dir = UP;
        break;
       case DirL:
           dir = LEFT;
        break;
       case DirD:
           dir = DOWN;
        break;
       case DirRD:
           dir = Vector2(cosf(-Seta45),sinf(-Seta45));
        break;
       case DirLD:
           dir = Vector2(cosf(PI+Seta45),sinf(PI+Seta45));
        break;
       case DirLU:
           dir = Vector2(cosf(PI-Seta45),sinf(PI-Seta45));
        break;
       case DirRU:
           dir = Vector2(cosf(Seta45),sinf(Seta45));
        break;
       case DirN:
           dir = Vector2(0.0f,0.0f);
        break;
    }

    pos += dir * 200.0f * DeltaTime;

    //����ġ ���
    foot.y = pos.y -15.0f;
    foot.x = pos.x;
    //���Ⱚ�� ����� ��ġ
    foot += dir * 7.0f;


    img->Pos = pos;
    img->update();
}

void Player::render()
{
    //����� ���ڿ�
    wstring str = L"�̸� ";
    //����� �ؽ�Ʈ �ڽ� (l,t,r,b)
    Vector2 ScreenPos = WorldToScreen(pos);
    RECT rect = { ScreenPos.x-20,ScreenPos.y -85,
        ScreenPos.x+20,ScreenPos.y -55 };
    Color brush = { 0.0f,1.0f,1.0f,1.0f };//������
                                          //�⺻
    WRITEMANAGER->RenderText(str, rect,20.0f,L"�������",
        brush);
    img->PlayAnimation(ANI_LOOP,true,0.3f);
    img->render();
}
