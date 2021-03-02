#pragma once
#include "stdafx.h"
//충돌체 우리가 충돌할려고 만든 필요한 변수
struct COL_CIRCLE
{
    Vector2 center;
    float   radius;
};
struct COL_RECT
{
    float left;
    float top;
    float right;
    float bottom;
};

static COL_RECT CreateColRect(Vector2 Pos, Vector2 Scale)
{
    COL_RECT temp;

    temp.left = Pos.x - Scale.x * 0.5f;
    temp.right = Pos.x + Scale.x * 0.5f;
    temp.top = Pos.y + Scale.y * 0.5f;
    temp.bottom = Pos.y - Scale.y * 0.5f;

    return temp;
}

static COL_CIRCLE CreateColCircle(Vector2 Pos, Vector2 Scale)
{
    COL_CIRCLE temp;

    temp.center.x = Pos.x;
    temp.center.y = Pos.y;
    temp.radius =  Scale.y * 0.5f;

    return temp;
}

static bool PtinRect(Vector2 Pt,COL_RECT rc)
{
    if(rc.left <=Pt.x && Pt.x< rc.right
        && rc.bottom <=Pt.y && Pt.y< rc.top)
    {
        return true;
    }
    return false;
}

static bool PtinCircle(Vector2 Pt,COL_CIRCLE cc)
{
    Vector2 Dis = cc.center - Pt;
    //벡터의 길이를 구한다
    float Length = D3DXVec2Length(&Dis);
    if(cc.radius >= Length)
    {
        return true;
    }
    return false;
}

//intersect교집합
//원과 원이 충돌했는가?
static bool intersectCircle(COL_CIRCLE cc1,COL_CIRCLE cc2)
{
    Vector2 Dis = cc1.center - cc2.center;
    //벡터의 길이를 구한다
    float Length = D3DXVec2Length(&Dis);

    if(cc1.radius+cc2.radius >= Length)
    {
        return true;
    }
    return false;
}
//네모와 네모 충돌 
static bool intersectRect(COL_RECT rc1,COL_RECT rc2)
{
    if(rc2.left < rc1.right && rc2.right > rc1.left&&
        rc2.top > rc1.bottom && rc2.bottom < rc1.top )
    {
        return true;
    }

    return false;
}

//네모와 원 충돌 
static bool intersectRectCircle(COL_RECT rc,COL_CIRCLE cc)
{
    //확장된 사각형
    COL_RECT temp1,temp2;

    //네모의 꼭지점
    Vector2 lt(rc.left,rc.top);
    Vector2 rt(rc.right,rc.top);
    Vector2 lb(rc.left,rc.bottom);
    Vector2 rb(rc.right,rc.bottom);

    //얕은복사
    temp1 = rc;
    temp1.top += cc.radius;
    temp1.bottom -=cc.radius;

    temp2 = rc;
    temp2.left -= cc.radius;
    temp2.right +=cc.radius;
    
    if(PtinRect(cc.center,temp1))
    {
        return true;
    }
    if(PtinRect(cc.center,temp2))
    {
        return true;
    }
    if(PtinCircle(lt,cc))
    {
        return true;
    }
    if(PtinCircle(lb,cc))
    {
        return true;
    }
    if(PtinCircle(rt,cc))
    {
        return true;
    }
    if(PtinCircle(rb,cc))
    {
        return true;
    }

    return false;
}