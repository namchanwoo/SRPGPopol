#pragma once
#include "stdafx.h"

static bool PtinRect(Vector2 Pt,Rect* rc)
{
    Matrix PlInverse;
    //                  ������   �����ҳ� �ִ³�
    D3DXMatrixInverse(&PlInverse, NULL, &rc->RT);
    COL_RECT colrc;

    D3DXVec2TransformCoord(&Pt,&Pt,&PlInverse);
    colrc.left = -rc->Scale.x * 0.5f;
    colrc.right = rc->Scale.x * 0.5f;
    colrc.top = rc->Scale.y * 0.5f;
    colrc.bottom = -rc->Scale.y * 0.5f;

    return PtinRect(Pt,colrc);
}

//�Լ� �����ε�
static bool intersectRectCircle(Rect* rc,Circle* cc)
{
    Matrix PlInverse;
    //                  ������   �����ҳ� �ִ³�
    D3DXMatrixInverse(&PlInverse, NULL, &rc->RT);

    COL_RECT colrc;
    COL_CIRCLE colcc = cc->Col;

    //���Ѵ� ��İ� �ݶ��̴���
    D3DXVec2TransformCoord(&colcc.center,&colcc.center,&PlInverse);

    //������ ��ġ�ϰ� �����Ƿ� ���������ݰ���ŭ ������ �ȴ�.
    colrc.left = -rc->Scale.x * 0.5f;
    colrc.right = rc->Scale.x * 0.5f;
    colrc.top = rc->Scale.y * 0.5f;
    colrc.bottom = -rc->Scale.y * 0.5f;

    return intersectRectCircle(colrc,colcc);
}
//ȸ���� �簢������ �浹
static bool intersectRect(Rect* rc1,Rect* rc2)
{
    //4���� �˻�
    //�߽������� ����
    Vector2 Center = rc1->Pos - rc2->Pos;

    //4���� ��
    //rc1->Right;rc1->Up;rc2->Right;rc2->Up;

    //rc1->Right ���� �������� ����
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
    //rc1->Up ���� �������� ����
    Dis = fabs(D3DXVec2Dot(&Center,&rc1->Up));

    LengthA = rc1->Scale.y*0.5f;
    LengthB = fabs(D3DXVec2Dot(&Vec1,&rc1->Up));
    LengthC = fabs(D3DXVec2Dot(&Vec2,&rc1->Up));
    LengthA = LengthA + LengthB + LengthC;
    if(Dis >LengthA)
    {
        return false;
    }

    //rc2->Right ���� �������� ����
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
    //rc2->Up ���� �������� ����
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



//�ٻ�Ÿ���� �𼭸�6�� ���ϱ�
static Vector2 HexCorner(Vector2 center,float size,int i)
{
	Vector2 var;
	float angle_deg = 60 * i+1 - 30;
	float angle_rad = PI / 180 * angle_deg;
	
	var.x = center.x + size * cos(angle_rad);
	var.y = center.y + size * sin(angle_rad);
	return var;

}

//�ٻ�Ÿ�ϰ� ���� �浹
static bool PtInHex(Vector2 pt,Tile& p,TileMap& tm) 
{
	//6���� ������ ����
	Vector2 polygon[6];

	//������ü�� 6���� �ݺ���
	for (int i = 0; i < 6; i++)
	{
		polygon[i] = HexCorner(p.Pos, tm.GetRefTileSize().x, i);
	}

	//crosses�� ��q�� ������ �������� �ٰ������� ������ ����
	//Ȧ���� ���ο� �ִ� �� ,¦���� �ܺο� �ִ� ��
	int crosses = 0;

	for (int i = 0; i < 6; i++) 
	{
		int j = (i + 1) % 6;
		//�� pt�� ���� (polygon[i], polygon[j])�� y��ǥ ���̿� ����
		if ((polygon[i].y > pt.y) != (polygon[j].y > pt.y))
		{

			//atX�� �� B�� ������ ���򼱰� ���� (polygon[i], polygon[j])�� ����
			double atX = (polygon[j].x - polygon[i].x)*(pt.y - polygon[i].y) / (polygon[j].y - polygon[i].y) + polygon[i].x;
			//atX�� ������ ���������� ������ ������ ������ ������ ������Ų��.
			if (pt.x < atX)
				crosses++;
		}
	}
	//Ȧ���� Ʈ��
	return crosses % 2 > 0;
}

//���� ���Ÿ�� �ȿ� �ִ°�?
static bool PtInTile(TileMap* tile, Vector2 Pt, OUT POINT& index)//�ۿ� ������ �ٲ�
{
	COL_RECT col;
	col.left = tile->GetRefLB().x;
	col.bottom = tile->GetRefLB().y;
	col.right = col.left + tile->GetRefTileMax().x * tile->GetRefTileSize().x;
	col.top = col.bottom + tile->GetRefTileMax().y * tile->GetRefTileSize().y;

	//�ƴ϶�� �ȿ� �ִ�.z`

	//�������� �̵��Ѹ�ŭ ���ֱ�
	Pt -= tile->GetRefLB();

	

	//�ε��� ���
	index.x = (LONG)(Pt.x / tile->GetRefTileSize().x);
	index.y = (LONG)(Pt.y / tile->GetRefTileSize().y);

	if (!PtinRect(Pt, col))
	{
		//Ÿ�� �ۿ� �ִ°Ŵ�.
		return false;
	}


	return true;
}


//���� ���Ÿ�� �ȿ� �ִ°�?
static bool PtInHexTile(TileMapEdit* edit, Vector2 Pt, OUT OffsetCoord& coord)//�ۿ� ������ �ٲ�
{

	COL_RECT col;
	col.left = 0;
	col.bottom = 0;

	
	col.right = col.left + edit->map.width * (edit->map.TileSize.x + edit->map.LB.x) + edit->map.TileSize.x;
	col.top = col.bottom + edit->map.height * (edit->map.TileSize.y + edit->map.LB.y) + edit->map.TileSize.y;



	

	//�������� �̵��Ѹ�ŭ ���ֱ�
	Pt -= edit->map.LB;

	//���콺�� ����Ʈ�� �ٻ���ǥ�� ��ȯ (��ȯ �������� doubleŸ���� ���� ��ȯ)
	FractionalHex curHex = edit->pixel_to_hex(edit->layout, Pt);
	//�÷�Ÿ���� ��縦 ��ƮŸ���� ���� ��ȯ
	Hex curHex2 = edit->hex_round(curHex);

	//coord = edit->cube_to_roffset(+1, curHex2);

	//�ش� ť����ǥ�� ��������ǥ ��ȯ �� �Է�
	OffsetCoord curCoord = edit->cube_to_roffset(+1, curHex2);
	if (curCoord.col > edit->map.width - 1 || curCoord.row > edit->map.height - 1)
	{
		coord = OffsetCoord();
	}
	else
	{
		coord = curCoord;
	}


	if (!PtinRect(Pt, col))
	{
		//Ÿ�� �ۿ� �ִ°Ŵ�.
		return false;
	}

	//q = x , r=z ,s =y



	


	return true;
}