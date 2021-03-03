#include "stdafx.h"
#include "TileMap.h"

UINT TileMap::findH(POINT from, POINT to)
{
	UINT Cost;
    //			�ε��� ������ ���밪�� *10 �Ȱ����� ���� ���´�.
    Cost = (abs(from.x - to.x) + abs(from.y - to.y))*10;

    return (UINT)Cost;
}

HRESULT TileMap::init()
{
	//�⺻ Ÿ������
	TileMax.y = 10;
	TileMax.x = 10;
    LB.x = 0.0f;
    LB.y = 0.0f;
	TileSize.x = 80.0f;
	TileSize.y = 80.0f;

	//Ÿ�� �Ҵ�
	ResizeTile();

	//Ÿ�ϵ� ��ġ ���
	InitPosition();

    return S_OK;
}

void TileMap::release()
{
	//Ÿ������ ���� �����
	ClearTile();

	//���Ϳ� �߰��� �̹��� �����
    for(UINT i=0; i< vecTile.size(); i++)
    {
        SAFE_DELETE(vecTile[i]);
    }
    //clear ������ ���ι���(capacity)�� ��������
    vecTile.clear();
    //�װ������ִ��Լ�
    vecTile.shrink_to_fit();
}

void TileMap::update()
{
    
}

void TileMap::render()
{
	//â ���� �Ʒ�, ������ �� ������ǥ
	Vector2 lb = MAINCAM->Pos;
	Vector2 rt;
	rt.x = lb.x + WINSIZEX;
	rt.y = lb.y + WINSIZEY;

	POINT LBIdx;
	PtInTile(this, lb, LBIdx);
	LBIdx.x = Saturate(LBIdx.x, 0L, TileMax.x - 1L);
	LBIdx.y = Saturate(LBIdx.y, 0L, TileMax.y - 1L);


	POINT RTIdx;
	PtInTile(this, rt, RTIdx);

	RTIdx.x = Saturate(RTIdx.x, 0L, TileMax.x - 1L);
	RTIdx.y = Saturate(RTIdx.y, 0L, TileMax.y - 1L);

	for (LONG i = LBIdx.x; i <= RTIdx.x; i++)
	{
		for (LONG j = LBIdx.y; j <= RTIdx.y; j++)
		{
            UINT vecindex = Tiles[i][j].vecIdx;
            vecTile[vecindex]->Scale = TileSize;
            vecTile[vecindex]->Pos = Tiles[i][j].Pos + LB;
            vecTile[vecindex]->update();
            vecTile[vecindex]->CurrentFrameX =
                Tiles[i][j].ImgIdx.x;
            vecTile[vecindex]->CurrentFrameY =
                Tiles[i][j].ImgIdx.y;
            if(Tiles[i][j].state == TILE_WALL)
            {
                vecTile[vecindex]->color = Color(0.7f,0,0,Alpha);
                vecTile[vecindex]->render();
            }
            else
            {
                vecTile[vecindex]->color = Color(1,1,1,Alpha); 
                vecTile[vecindex]->render();
            }
            vecTile[vecindex]->render();
        }
    }

	//for (UINT i = 0; i < TileMax.x; i++)
	//{
	//	for (UINT j = 0; j < TileMax.y; j++)
	//	{
	//		UINT vecindex = Tiles[i][j].vecIdx;
	//		vecTile[vecindex]->Pos = Tiles[i][j].Pos + LB;
	//		vecTile[vecindex]->update();
	//		vecTile[vecindex]->CurrentFrameX =
	//			Tiles[i][j].ImgIdx.x;
	//		vecTile[vecindex]->CurrentFrameY =
	//			Tiles[i][j].ImgIdx.y;
	//		vecTile[vecindex]->render();
	//	}
	//}
}

void TileMap::AddImage(_tstring file, UINT MaxFrameX, UINT MaxFrameY, string vs, string ps)
{
    Image* temp = new Image();
    temp->init(file,vs,ps);
    temp->Scale = TileSize;
    temp->MaxFrameX = MaxFrameX;
    temp->MaxFrameY = MaxFrameY;
    vecTile.emplace_back(temp);
}

void TileMap::InitPosition()
{

	Vector2 halfScale = TileSize * 0.5f;
	for (LONG i = 0; i < TileMax.x; i++)
	{
		for (LONG j = 0; j < TileMax.y; j++)
		{
			//��ü������ Ÿ�� ��ġ �ٽ����
			Tiles[i][j].Pos.x = halfScale.x + (TileSize.x * i);
			Tiles[i][j].Pos.y = halfScale.y + (TileSize.y * j);
			Tiles[i][j].TileIdx = POINT{ i,j };
		}
	}
    
}

void TileMap::ClearTile()
{
	//����ũ�⸦ �ƽ� �����ŭ �����
	for (LONG i = 0; i < TileMax.x; i++)
	{
		Tiles[i].clear();
		Tiles[i].shrink_to_fit();
	}
	Tiles.clear();
	Tiles.shrink_to_fit();
}

void TileMap::ResizeTile()
{
	//����ũ�⸦ �ƽ� �����ŭ ��´�
	Tiles.resize(TileMax.x);
	for (LONG i = 0; i < TileMax.x; i++)
	{
		Tiles[i].resize(TileMax.y);
	}
}

//                            ���,       ����,        ��ȯ�� ��
bool TileMap::PathFinding(POINT from, POINT to, vector<Tile*>& way)
{
    //���߿� �ϳ��� ���̸� �� �� �ִ±��� ����.
    if(Tiles[to.x][to.y].state == TILE_WALL||
        Tiles[from.x][from.y].state == TILE_WALL)
    {
        return false;
    }
    //������ �ִ� ���� ���� ����.
    way.clear();
    //����� ������ ������
    if(to.x == from.x && to.y == from.y)
    {
        return false;
    }

    //Ÿ�� ��� �ʱ�ȭ
    for(int i=0; i< TileMax.x; i++)
    {
        for(int j=0; j< TileMax.y; j++)
        {
            //�ִ밪 �ʱ�ȭ
            Tiles[i][j].F = UINT_MAX;
            Tiles[i][j].G = UINT_MAX;
            Tiles[i][j].H = UINT_MAX;
            Tiles[i][j].P = nullptr;
            Tiles[i][j].isFind = false;
        }
    }
   
    //���Ž�� ����

    //Ž�� ����Ʈ �����
    list<Tile*> FindList;
    //��������� ���� ����Ʈ�� �߰�
    FindList.emplace_back(&Tiles[from.x][from.y]);
    
    //������� ��� �����
    Tiles[from.x][from.y].G = 0;
    Tiles[from.x][from.y].H = findH(from,to);
    Tiles[from.x][from.y].F = Tiles[from.x][from.y].G + Tiles[from.x][from.y].H;
    
    //����Ʈ�� ������� ���������� �ݺ�
    while(1)
    {
        //�������� ��ã�� ����
        if(Tiles[to.x][to.y].F != UINT_MAX)
        {
            break;
        }
        //����Ʈ�� ����ִٸ� ��ã�� ����
        if(FindList.empty())
        {
            way.clear();
            return false;
        }
        //����Ʈ���� F���� ���� ����Ÿ��
		Tile* MinF;
        //ó���� �� �հ����� �ޱ�
        MinF = FindList.front();

		//������ ���
		list<Tile*>::iterator it;
		it = FindList.begin();

        //����Ʈ ������ Ž��
        for(auto i = FindList.begin();
            i != FindList.end(); i++)
        {
            //�ּҺ�� �ٲٱ�
            if(MinF->F > (*i)->F)
            {
                MinF = *i;
				it = i;
            }
        }

		//Ž���̳��� �ּҺ��Ÿ���� ���̻�
		//����Ʈ�ȿ� �������� ����
		MinF->isFind = true;
		FindList.erase(it);


        //�ּҺ���� Ÿ���� ã�Ҵٸ� 8������ ��밪����

        //4����

        //����
        POINT left = POINT{MinF->TileIdx.x -1,
            MinF->TileIdx.y};
        //-x�ε����� �����Ƿ�0���� Ŭ����
        if(left.x >=0)
        {
            //���� �ƴҶ�
            //�����Ϸ��� Ÿ���� ���� g������ 
            //�ּҺ��Ÿ�Ͽ��� ���º���� �� ������
           if(Tiles[left.x][left.y].state != TILE_WALL &&
               Tiles[left.x][left.y].G > MinF->G + 10.0f)
           {
               //���� ��� ����
               Tiles[left.x][left.y].G = MinF->G + 10;
               //���� ��� ����
               Tiles[left.x][left.y].H = findH(left,to);
               //�� ��� ����
               Tiles[left.x][left.y].F = MinF->G + 10 +
                   Tiles[left.x][left.y].H;
               //������ ���� ���� �Ǿ��°�?
               Tiles[left.x][left.y].P = MinF;
               //Ž������Ʈ�� �߰�
               if(Tiles[left.x][left.y].isFind == false)
               {
                   FindList.emplace_back(&Tiles[left.x][left.y]);
               }
           }
        }
        //������
        POINT right = POINT{MinF->TileIdx.x +1,
            MinF->TileIdx.y};
        //-x�ε����� �����Ƿ�0���� Ŭ����
        if(right.x < TileMax.x)
        {
            //���� �ƴҶ�
            //�����Ϸ��� Ÿ���� ���� g������ 
            //�ּҺ��Ÿ�Ͽ��� ���º���� �� ������
            if(Tiles[right.x][right.y].state != TILE_WALL &&
                Tiles[right.x][right.y].G > MinF->G + 10)
            {
                //���� ��� ����
                Tiles[right.x][right.y].G = MinF->G + 10;
                //���� ��� ����
                Tiles[right.x][right.y].H = findH(right,to);
                //�� ��� ����
                Tiles[right.x][right.y].F = MinF->G + 10 +
                    Tiles[right.x][right.y].H;
                //������ ���� ���� �Ǿ��°�?
                Tiles[right.x][right.y].P = MinF;
                //Ž������Ʈ�� �߰�
                if(Tiles[right.x][right.y].isFind == false)
                {
                    FindList.emplace_back(&Tiles[right.x][right.y]);
                }
            }
        }
        //����
        POINT top = POINT{MinF->TileIdx.x,
            MinF->TileIdx.y+1};
        //-x�ε����� �����Ƿ�0���� Ŭ����
        if(top.y <TileMax.y)
        {
            //���� �ƴҶ�
            //�����Ϸ��� Ÿ���� ���� g������ 
            //�ּҺ��Ÿ�Ͽ��� ���º���� �� ������
            if(Tiles[top.x][top.y].state != TILE_WALL &&
                Tiles[top.x][top.y].G > MinF->G + 10)
            {
                //���� ��� ����
                Tiles[top.x][top.y].G = MinF->G + 10;
                //���� ��� ����
                Tiles[top.x][top.y].H = findH(top,to);
                //�� ��� ����
                Tiles[top.x][top.y].F = MinF->G + 10 +
                    Tiles[top.x][top.y].H;
                //������ ���� ���� �Ǿ��°�?
                Tiles[top.x][top.y].P = MinF;
                //Ž������Ʈ�� �߰�
                if(Tiles[top.x][top.y].isFind == false)
                {
                    FindList.emplace_back(&Tiles[top.x][top.y]);
                }
            }
        }
        //�Ʒ���
        POINT bottom = POINT{MinF->TileIdx.x,
            MinF->TileIdx.y-1};
        //-x�ε����� �����Ƿ�0���� Ŭ����
        if(bottom.y >= 0)
        {
            //���� �ƴҶ�
            //�����Ϸ��� Ÿ���� ���� g������ 
            //�ּҺ��Ÿ�Ͽ��� ���º���� �� ������
            if(Tiles[bottom.x][bottom.y].state != TILE_WALL &&
                Tiles[bottom.x][bottom.y].G > MinF->G + 10)
            {
                //���� ��� ����
                Tiles[bottom.x][bottom.y].G = MinF->G + 10;
                //���� ��� ����
                Tiles[bottom.x][bottom.y].H = findH(right,to);
                //�� ��� ����
                Tiles[bottom.x][bottom.y].F = MinF->G + 10 +
                    Tiles[bottom.x][bottom.y].H;
                //������ ���� ���� �Ǿ��°�?
                Tiles[bottom.x][bottom.y].P = MinF;
                //Ž������Ʈ�� �߰�
                if(Tiles[bottom.x][bottom.y].isFind == false)
                {
                    FindList.emplace_back(&Tiles[bottom.x][bottom.y]);
                }
            }
        }
        //4���� ��
        
    }//while�� ����
	FindList.clear();

    //�������������� ��������� way�� pushback
	Tile* TileWay = &Tiles[to.x][to.y];
    while(1)
    {
        //��������
        if(TileWay->TileIdx.x == from.x&&
            TileWay->TileIdx.y == from.y)
        {
            break;
        }
        //�濡 �߰�
        way.emplace_back(TileWay);
        //���� �߰��� Ÿ���� �θ�� ��´�.
        TileWay = TileWay->P;
    }
    return true;
}

