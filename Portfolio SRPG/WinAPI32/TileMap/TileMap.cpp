#include "stdafx.h"
#include "TileMap.h"

UINT TileMap::findH(POINT from, POINT to)
{
	UINT Cost;
    //			인덱스 차이의 절대값에 *10 된값으로 값을 갖는다.
    Cost = (abs(from.x - to.x) + abs(from.y - to.y))*10;

    return (UINT)Cost;
}

HRESULT TileMap::init()
{
	//기본 타일정보
	TileMax.y = 10;
	TileMax.x = 10;
    LB.x = 0.0f;
    LB.y = 0.0f;
	TileSize.x = 80.0f;
	TileSize.y = 80.0f;

	//타일 할당
	ResizeTile();

	//타일들 위치 잡기
	InitPosition();

    return S_OK;
}

void TileMap::release()
{
	//타일정보 전부 지우기
	ClearTile();

	//벡터에 추가한 이미지 지우기
    for(UINT i=0; i< vecTile.size(); i++)
    {
        SAFE_DELETE(vecTile[i]);
    }
    //clear 했을때 내부버퍼(capacity)가 안지워짐
    vecTile.clear();
    //그걸지워주는함수
    vecTile.shrink_to_fit();
}

void TileMap::update()
{
    
}

void TileMap::render()
{
	//창 왼쪽 아래, 오른쪽 위 월드좌표
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
			//전체적으로 타일 위치 다시잡기
			Tiles[i][j].Pos.x = halfScale.x + (TileSize.x * i);
			Tiles[i][j].Pos.y = halfScale.y + (TileSize.y * j);
			Tiles[i][j].TileIdx = POINT{ i,j };
		}
	}
    
}

void TileMap::ClearTile()
{
	//벡터크기를 맥스 사이즈만큼 지운다
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
	//벡터크기를 맥스 사이즈만큼 잡는다
	Tiles.resize(TileMax.x);
	for (LONG i = 0; i < TileMax.x; i++)
	{
		Tiles[i].resize(TileMax.y);
	}
}

//                            출발,       도착,        반환할 길
bool TileMap::PathFinding(POINT from, POINT to, vector<Tile*>& way)
{
    //둘중에 하나가 벽이면 갈 수 있는길이 없다.
    if(Tiles[to.x][to.y].state == TILE_WALL||
        Tiles[from.x][from.y].state == TILE_WALL)
    {
        return false;
    }
    //기존에 있던 길은 전부 비운다.
    way.clear();
    //출발지 목적지 같으면
    if(to.x == from.x && to.y == from.y)
    {
        return false;
    }

    //타일 비용 초기화
    for(int i=0; i< TileMax.x; i++)
    {
        for(int j=0; j< TileMax.y; j++)
        {
            //최대값 초기화
            Tiles[i][j].F = UINT_MAX;
            Tiles[i][j].G = UINT_MAX;
            Tiles[i][j].H = UINT_MAX;
            Tiles[i][j].P = nullptr;
            Tiles[i][j].isFind = false;
        }
    }
   
    //비용탐색 시작

    //탐색 리스트 만들기
    list<Tile*> FindList;
    //출발지부터 먼저 리스트에 추가
    FindList.emplace_back(&Tiles[from.x][from.y]);
    
    //출발지의 비용 만들기
    Tiles[from.x][from.y].G = 0;
    Tiles[from.x][from.y].H = findH(from,to);
    Tiles[from.x][from.y].F = Tiles[from.x][from.y].G + Tiles[from.x][from.y].H;
    
    //리스트가 비어있지 않을때까지 반복
    while(1)
    {
        //종료조건 길찾기 성공
        if(Tiles[to.x][to.y].F != UINT_MAX)
        {
            break;
        }
        //리스트가 비어있다면 길찾기 종료
        if(FindList.empty())
        {
            way.clear();
            return false;
        }
        //리스트에서 F값이 가장 작은타일
		Tile* MinF;
        //처음엔 맨 앞값으로 받기
        MinF = FindList.front();

		//지워줄 대상
		list<Tile*>::iterator it;
		it = FindList.begin();

        //리스트 끝까지 탐색
        for(auto i = FindList.begin();
            i != FindList.end(); i++)
        {
            //최소비용 바꾸기
            if(MinF->F > (*i)->F)
            {
                MinF = *i;
				it = i;
            }
        }

		//탐색이끝난 최소비용타일은 더이상
		//리스트안에 못들어오게 막기
		MinF->isFind = true;
		FindList.erase(it);


        //최소비용의 타일을 찾았다면 8방향의 비용값갱신

        //4방향

        //왼쪽
        POINT left = POINT{MinF->TileIdx.x -1,
            MinF->TileIdx.y};
        //-x인덱스는 없으므로0보다 클때만
        if(left.x >=0)
        {
            //벽이 아닐때
            //갱신하려는 타일의 원래 g값보다 
            //최소비용타일에서 오는비용이 더 낮을때
           if(Tiles[left.x][left.y].state != TILE_WALL &&
               Tiles[left.x][left.y].G > MinF->G + 10.0f)
           {
               //현재 비용 갱신
               Tiles[left.x][left.y].G = MinF->G + 10;
               //예상 비용 갱신
               Tiles[left.x][left.y].H = findH(left,to);
               //총 비용 갱신
               Tiles[left.x][left.y].F = MinF->G + 10 +
                   Tiles[left.x][left.y].H;
               //누구로 부터 갱신 되었는가?
               Tiles[left.x][left.y].P = MinF;
               //탐색리스트에 추가
               if(Tiles[left.x][left.y].isFind == false)
               {
                   FindList.emplace_back(&Tiles[left.x][left.y]);
               }
           }
        }
        //오른쪽
        POINT right = POINT{MinF->TileIdx.x +1,
            MinF->TileIdx.y};
        //-x인덱스는 없으므로0보다 클때만
        if(right.x < TileMax.x)
        {
            //벽이 아닐때
            //갱신하려는 타일의 원래 g값보다 
            //최소비용타일에서 오는비용이 더 낮을때
            if(Tiles[right.x][right.y].state != TILE_WALL &&
                Tiles[right.x][right.y].G > MinF->G + 10)
            {
                //현재 비용 갱신
                Tiles[right.x][right.y].G = MinF->G + 10;
                //예상 비용 갱신
                Tiles[right.x][right.y].H = findH(right,to);
                //총 비용 갱신
                Tiles[right.x][right.y].F = MinF->G + 10 +
                    Tiles[right.x][right.y].H;
                //누구로 부터 갱신 되었는가?
                Tiles[right.x][right.y].P = MinF;
                //탐색리스트에 추가
                if(Tiles[right.x][right.y].isFind == false)
                {
                    FindList.emplace_back(&Tiles[right.x][right.y]);
                }
            }
        }
        //위쪽
        POINT top = POINT{MinF->TileIdx.x,
            MinF->TileIdx.y+1};
        //-x인덱스는 없으므로0보다 클때만
        if(top.y <TileMax.y)
        {
            //벽이 아닐때
            //갱신하려는 타일의 원래 g값보다 
            //최소비용타일에서 오는비용이 더 낮을때
            if(Tiles[top.x][top.y].state != TILE_WALL &&
                Tiles[top.x][top.y].G > MinF->G + 10)
            {
                //현재 비용 갱신
                Tiles[top.x][top.y].G = MinF->G + 10;
                //예상 비용 갱신
                Tiles[top.x][top.y].H = findH(top,to);
                //총 비용 갱신
                Tiles[top.x][top.y].F = MinF->G + 10 +
                    Tiles[top.x][top.y].H;
                //누구로 부터 갱신 되었는가?
                Tiles[top.x][top.y].P = MinF;
                //탐색리스트에 추가
                if(Tiles[top.x][top.y].isFind == false)
                {
                    FindList.emplace_back(&Tiles[top.x][top.y]);
                }
            }
        }
        //아래쪽
        POINT bottom = POINT{MinF->TileIdx.x,
            MinF->TileIdx.y-1};
        //-x인덱스는 없으므로0보다 클때만
        if(bottom.y >= 0)
        {
            //벽이 아닐때
            //갱신하려는 타일의 원래 g값보다 
            //최소비용타일에서 오는비용이 더 낮을때
            if(Tiles[bottom.x][bottom.y].state != TILE_WALL &&
                Tiles[bottom.x][bottom.y].G > MinF->G + 10)
            {
                //현재 비용 갱신
                Tiles[bottom.x][bottom.y].G = MinF->G + 10;
                //예상 비용 갱신
                Tiles[bottom.x][bottom.y].H = findH(right,to);
                //총 비용 갱신
                Tiles[bottom.x][bottom.y].F = MinF->G + 10 +
                    Tiles[bottom.x][bottom.y].H;
                //누구로 부터 갱신 되었는가?
                Tiles[bottom.x][bottom.y].P = MinF;
                //탐색리스트에 추가
                if(Tiles[bottom.x][bottom.y].isFind == false)
                {
                    FindList.emplace_back(&Tiles[bottom.x][bottom.y]);
                }
            }
        }
        //4방향 끝
        
    }//while문 종료
	FindList.clear();

    //도착지에서부터 출발지까지 way에 pushback
	Tile* TileWay = &Tiles[to.x][to.y];
    while(1)
    {
        //종료조건
        if(TileWay->TileIdx.x == from.x&&
            TileWay->TileIdx.y == from.y)
        {
            break;
        }
        //길에 추가
        way.emplace_back(TileWay);
        //다음 추가할 타일을 부모로 잡는다.
        TileWay = TileWay->P;
    }
    return true;
}

