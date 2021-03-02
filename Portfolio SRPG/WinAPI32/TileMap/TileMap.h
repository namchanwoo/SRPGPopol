#pragma once
enum TileState
{
    TILE_NONE,
    TILE_WALL
};


//큐브 측 좌표
typedef struct CubeOfSet
{
	INT x;
	INT y;
	INT z;
}CUBEOFSET;

//타일마다 가져야할 정보
struct HexTile
{

#pragma region A* 알고리즘에 필요한 변수들
	UINT F;					 //예상비용 + 현재까지비용
	UINT G;					 //현재까지비용
	UINT H;					 //예상비용
	HexTile* P;				 //누굴통해 갱신되었는가?
#pragma endregion

    
    POINT TileIdx;			 //본인 타일 인덱스
	CubeOfSet CubeIdx;		 //큐브측 좌표


    bool  isFind;			 //탐색을 했던 타일인가?

    UINT vecIdx;			 //몇번째 이미지?
    POINT ImgIdx;			 //몇번째 분할?

    int state;				 //타일 상태
    Vector2 Pos;			 //타일의 위치 LB에서 떨어진

	HexTile():state(TILE_NONE),vecIdx(0)
    {
        ImgIdx.x = 0;
        ImgIdx.y = 0;
		
    }


	//큐브좌표를 오프셋으로
	POINT CubeToOddr(CUBEOFSET Idx)
	{
		POINT var;
		var.x = Idx.x + (Idx.z - (Idx.z & 1)) / 2;
		var.y = Idx.z;
		return var;
	}

	//오프셋 좌표를 큐브좌표로
	CubeOfSet OddrToCube(POINT Idx)
	{
		CubeOfSet var;
		var.x = Idx.x - (Idx.y - (Idx.y & 1)) / 2;
		var.z = Idx.y;
		var.y = -var.x -var.z;
		return var;
	}


};

class TileMap
{
private:

    //여러장의 이미지를 사용할수 있으므로
    vector<Image*>  vecTile;

    // 한 타일의 가로세로 크기
    Vector2         TileSize;

    // 타일의 가로세로 갯수
    POINT           TileMax;

    //타일의 기준이되는 왼쪽 아래점 위치
    Vector2         LB;
   
	//정보를 담아줄 타일
	vector<vector<HexTile>>    Tiles;

	//투명도 값
    float           Alpha = 1.0f;

	//맨하탄방식으로 도착예상비용 계산
    UINT findH(POINT from,POINT to);


public:
    
    HRESULT init();
    void release();
    void update();
    void render();
	//타일맵에 쓰일 이미지 추가
    void AddImage(_tstring file, UINT MaxFrameX, UINT MaxFrameY,
        string vs = "VS", string ps = "PS");
	//타일위치 다시잡기
    void InitPosition();
	//타일 전부 해제
	void ClearTile();
	//타일 재 할당
	void ResizeTile();

    //길찾기
    bool PathFinding(POINT from,POINT to,
        vector<HexTile*>& way);
    
	//참조형 겟 함수
    MAKEREFGET(Vector2 , TileSize)
    MAKEREFGET(POINT  ,TileMax)
    MAKEREFGET(Vector2  ,LB)
    MAKEREFGET(vector<Image*>  ,vecTile)
    MAKEREFGET(vector<vector<HexTile>>  ,  Tiles)
    MAKEREFGET(float  ,Alpha)
};



