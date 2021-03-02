#include "stdafx.h"
#include "Enemy.h"


HRESULT Enemy::init()
{
    img = new Image();
    img->init(_T("boss.bmp"));
    img->Scale = Vector2(100.0f,100.0f);
    img->MaxFrameX = 1;
    img->MaxFrameY = 8;
    pos = Vector2(400.0f,300.0f);

    PlDir = bDirN;
    Seta45 = PI/4.0f;
    Seta225= PI/8.0f;

    BossState = bNONE;
    return S_OK;
}

void Enemy::release()
{
}

void Enemy::update()
{
    time += DeltaTime;
    if(time > 1.0f)
    {
        time-= 1.0f;
        img->color = Color(RndFloat(),RndFloat(),RndFloat(),1.0f);
    }


    dir = pl->GetRefpos() - pos;

    seta = GetRadian(dir);
    float len = D3DXVec2Length(&dir);

    
    
    if(len<=100.0f)
    {
        BossState = bATTACK;
    }
    else if(len<=150.0f)
    {
        BossState = bFOLLOW;
    }
    else if(len<=200.0f)
    {
        BossState = bLOOK;
    }
    else
    {
        BossState = bNONE;
    }
    //인지 했을때
    if(BossState >= bLOOK)
    {
        if(Seta225<=seta&&seta < Seta225 + Seta45)
        {
            PlDir = bDirRU;
        }
        else if(Seta225+ Seta45<=seta&&seta < Seta225 + Seta45*2.0f)
        {
            PlDir = bDirU;
        }
        else if(Seta225+ Seta45*2.0f<=seta&&seta < Seta225 + Seta45*3.0f)
        {
            PlDir = bDirLU;
        }
        else if(Seta225+ Seta45*3.0f<=seta&&seta < Seta225 + Seta45*4.0f)
        {
            PlDir = bDirL;
        }
        else if(Seta225+ Seta45*4.0f<=seta&&seta < Seta225 + Seta45*5.0f)
        {
            PlDir = bDirLD;
        }
        else if(Seta225+ Seta45*5.0f<=seta&&seta < Seta225 + Seta45*6.0f)
        {
            PlDir = bDirD;
        }
        else if(Seta225+ Seta45*6.0f<=seta&&seta < Seta225 + Seta45*7.0f)
        {
            PlDir = bDirRD;
        }
        else
        {
            PlDir = bDirR;
        }
    }
    

    img->CurrentFrameY = PlDir;

    switch(PlDir)
    {
        case bDirR:
            dir = RIGHT;
            break;
        case bDirU:
            dir = UP;
            break;
        case bDirL:
            dir = LEFT;
            break;
        case bDirD:
            dir = DOWN;
            break;
        case bDirRD:
            dir = Vector2(cosf(-Seta45),sinf(-Seta45));
            break;
        case bDirLD:
            dir = Vector2(cosf(PI+Seta45),sinf(PI+Seta45));
            break;
        case bDirLU:
            dir = Vector2(cosf(PI-Seta45),sinf(PI-Seta45));
            break;
        case bDirRU:
            dir = Vector2(cosf(Seta45),sinf(Seta45));
            break;
    }
    if(BossState == bFOLLOW)
    {
        pos += dir * 20.0f * DeltaTime;
    }
    img->Pos = pos;
    img->update();
}

void Enemy::render()
{
    img->render();
}
