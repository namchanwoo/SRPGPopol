#pragma once
#include "stdafx.h"

static bool PtinRect(Vector2 Pt,Rect* rc)
{
    Matrix PlInverse;
    //                  받을놈   무시할놈 주는놈
    D3DXMatrixInverse(&PlInverse, NULL, &rc->RT);
    COL_RECT colrc;

    D3DXVec2TransformCoord(&Pt,&Pt,&PlInverse);
    colrc.left = -rc->Scale.x * 0.5f;
    colrc.right = rc->Scale.x * 0.5f;
    colrc.top = rc->Scale.y * 0.5f;
    colrc.bottom = -rc->Scale.y * 0.5f;

    return PtinRect(Pt,colrc);
}

//함수 오버로딩
static bool intersectRectCircle(Rect* rc,Circle* cc)
{
    Matrix PlInverse;
    //                  받을놈   무시할놈 주는놈
    D3DXMatrixInverse(&PlInverse, NULL, &rc->RT);

    COL_RECT colrc;
    COL_CIRCLE colcc = cc->Col;

    //곱한다 행렬과 콜라이더를
    D3DXVec2TransformCoord(&colcc.center,&colcc.center,&PlInverse);

    //원점에 위치하고 있으므로 스케일절반값만큼 잡으면 된다.
    colrc.left = -rc->Scale.x * 0.5f;
    colrc.right = rc->Scale.x * 0.5f;
    colrc.top = rc->Scale.y * 0.5f;
    colrc.bottom = -rc->Scale.y * 0.5f;

    return intersectRectCircle(colrc,colcc);
}
//회전된 사각형간의 충돌
static bool intersectRect(Rect* rc1,Rect* rc2)
{
    //4개축 검사
    //중심점간의 벡터
    Vector2 Center = rc1->Pos - rc2->Pos;

    //4개의 축
    //rc1->Right;rc1->Up;rc2->Right;rc2->Up;

    //rc1->Right 축을 기준으로 투영
    float Dis = fabs(D3DXVec2Dot(&Center,&rc1->Right));

    float LengthA = rc1->Scale.x*0.5f;
    Vector2 Vec1  = rc2->Up * rc2->Scale.y*0.5f;
    Vector2 Vec2  = rc2->Right * rc2->Scale.x*0.5f;
    float LengthB = fabs(D3DXVec2Dot(&Vec1,&rc1->Right));
    float LengthC = fabs(D3DXVec2Dot(&Vec2,&rc1->Right));
    LengthA = LengthA + LengthB + LengthC;
    if(Dis >LengthA)
    {
        return false;
    }
    //rc1->Up 축을 기준으로 투영
    Dis = fabs(D3DXVec2Dot(&Center,&rc1->Up));

    LengthA = rc1->Scale.y*0.5f;
    LengthB = fabs(D3DXVec2Dot(&Vec1,&rc1->Up));
    LengthC = fabs(D3DXVec2Dot(&Vec2,&rc1->Up));
    LengthA = LengthA + LengthB + LengthC;
    if(Dis >LengthA)
    {
        return false;
    }

    //rc2->Right 축을 기준으로 투영
    Dis = fabs(D3DXVec2Dot(&Center,&rc2->Right));

    LengthA = rc2->Scale.x*0.5f;
    Vec1  = rc1->Up * rc1->Scale.y*0.5f;
    Vec2  = rc1->Right * rc1->Scale.x*0.5f;
    LengthB = fabs(D3DXVec2Dot(&Vec1,&rc2->Right));
    LengthC = fabs(D3DXVec2Dot(&Vec2,&rc2->Right));
    LengthA = LengthA + LengthB + LengthC;
    if(Dis >LengthA)
    {
        return false;
    }
    //rc2->Up 축을 기준으로 투영
    Dis = fabs(D3DXVec2Dot(&Center,&rc2->Up));

    LengthA = rc2->Scale.y*0.5f;
    LengthB = fabs(D3DXVec2Dot(&Vec1,&rc2->Up));
    LengthC = fabs(D3DXVec2Dot(&Vec2,&rc2->Up));
    LengthA = LengthA + LengthB + LengthC;
    if(Dis >LengthA)
    {
        return false;
    }

    return true;
    
}
static bool PtinCircle(Vector2 Pt,Circle* cc)
{
    return PtinCircle(Pt,cc->Col);
}
static bool intersectCircle(Circle* cc1,Circle* cc2)
{
    return intersectCircle(cc1->Col,cc2->Col);
}
//
////점이 타일 안에 있는가?
//static bool PtInTile(TileMap* tile,Vector2 Pt,OUT POINT& index)//밖에 나가서 바뀔값
//{
//    COL_RECT col;
//    col.left = tile->GetRefLB().x;
//    col.bottom = tile->GetRefLB().y;
//    col.right = col.left+ tile->GetRefTileMax().x * tile->GetRefTileSize().x;
//    col.top = col.bottom+ tile->GetRefTileMax().y * tile->GetRefTileSize().y;
//
//    //아니라면 안에 있다.
//
//    //왼쪽점이 이동한만큼 빼주기
//    Pt -= tile->GetRefLB();
//    //인덱스 잡기
//    index.x = (LONG)(Pt.x / tile->GetRefTileSize().x);
//    index.y = (LONG)(Pt.y / tile->GetRefTileSize().y);
//
//    if(!PtinRect(Pt,col))
//    {
//        //타일 밖에 있는거다.
//        return false;
//    }
//    
//
//    return true;
//}


//핵사타일의 모서리6개 구하기
static Vector2 HexCorner(Vector2 center,float size,int i)
{
	Vector2 var;
	float angle_deg = 60 * i+1 - 30;
	float angle_rad = PI / 180 * angle_deg;
	
	var.x = center.x + size * cos(angle_rad);
	var.y = center.y + size * sin(angle_rad);
	return var;

}

//핵사타일과 점의 충돌
static bool PtInHexTile(Vector2 pt,Tile& p,TileMap& tm) 
{
	//6개의 꼭지점 변수
	Vector2 polygon[6];

	//정육면체라서 6개의 반복문
	for (int i = 0; i < 6; i++)
	{
		polygon[i] = HexCorner(p.Pos, tm.GetRefTileSize().x, i);
	}

	//crosses는 점q와 오른쪽 반직선과 다각형과의 교점의 개수
	//홀수면 내부에 있는 점 ,짝수면 외부에 있는 점
	int crosses = 0;

	for (int i = 0; i < 6; i++) 
	{
		int j = (i + 1) % 6;
		//점 pt가 선분 (polygon[i], polygon[j])의 y좌표 사이에 있음
		if ((polygon[i].y > pt.y) != (polygon[j].y > pt.y))
		{

			//atX는 점 B를 지나는 수평선과 선분 (polygon[i], polygon[j])의 교점
			double atX = (polygon[j].x - polygon[i].x)*(pt.y - polygon[i].y) / (polygon[j].y - polygon[i].y) + polygon[i].x;
			//atX가 오른쪽 반직선과의 교점이 맞으면 교점의 개수를 증가시킨다.
			if (pt.x < atX)
				crosses++;
		}
	}
	//홀수면 트루
	return crosses % 2 > 0;
}



//점이 헥사타일 안에 있는가?
static bool PtInTile(TileMap* tile, Vector2 Pt, OUT POINT& index)//밖에 나가서 바뀔값
{
	COL_RECT col;
	col.left = tile->GetRefLB().x;
	col.bottom = tile->GetRefLB().y;
	col.right = col.left + tile->GetRefTileMax().x * tile->GetRefTileSize().x;
	col.top = col.bottom + tile->GetRefTileMax().y * tile->GetRefTileSize().y;

	//아니라면 안에 있다.z`

	//왼쪽점이 이동한만큼 빼주기
	Pt -= tile->GetRefLB();

	for (int i = 0; i < tile->GetRefTileMax().x; i++)
	{
		for (int j = 0; j < tile->GetRefTileMax().y; j++)
		{
			if (PtInHexTile(Pt,tile->GetRefTiles()[i][j],*tile))
			{
				index.x = tile->GetRefTiles()[i][j].TileIdx.x;
				index.y = tile->GetRefTiles()[i][j].TileIdx.y;
					break;
			}
		}
	}

	//인덱스 잡기
	index.x = (LONG)(Pt.x / tile->GetRefTileSize().x);
	index.y = (LONG)(Pt.y / tile->GetRefTileSize().y);

	if (!PtinRect(Pt, col))
	{
		//타일 밖에 있는거다.
		return false;
	}


	return true;
}