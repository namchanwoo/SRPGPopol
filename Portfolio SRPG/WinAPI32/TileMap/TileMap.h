#pragma once
enum TileState
{
    TILE_NONE,
    TILE_WALL
};


//ť�� �� ��ǥ
typedef struct CubeOfSet
{
	INT x;
	INT y;
	INT z;
}CUBEOFSET;

//Ÿ�ϸ��� �������� ����
struct HexTile
{

#pragma region A* �˰��� �ʿ��� ������
	UINT F;					 //������ + ����������
	UINT G;					 //����������
	UINT H;					 //������
	HexTile* P;				 //�������� ���ŵǾ��°�?
#pragma endregion

    
    POINT TileIdx;			 //���� Ÿ�� �ε���
	CubeOfSet CubeIdx;		 //ť���� ��ǥ


    bool  isFind;			 //Ž���� �ߴ� Ÿ���ΰ�?

    UINT vecIdx;			 //���° �̹���?
    POINT ImgIdx;			 //���° ����?

    int state;				 //Ÿ�� ����
    Vector2 Pos;			 //Ÿ���� ��ġ LB���� ������

	HexTile():state(TILE_NONE),vecIdx(0)
    {
        ImgIdx.x = 0;
        ImgIdx.y = 0;
		
    }


	//ť����ǥ�� ����������
	POINT CubeToOddr(CUBEOFSET Idx)
	{
		POINT var;
		var.x = Idx.x + (Idx.z - (Idx.z & 1)) / 2;
		var.y = Idx.z;
		return var;
	}

	//������ ��ǥ�� ť����ǥ��
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

    //�������� �̹����� ����Ҽ� �����Ƿ�
    vector<Image*>  vecTile;

    // �� Ÿ���� ���μ��� ũ��
    Vector2         TileSize;

    // Ÿ���� ���μ��� ����
    POINT           TileMax;

    //Ÿ���� �����̵Ǵ� ���� �Ʒ��� ��ġ
    Vector2         LB;
   
	//������ ����� Ÿ��
	vector<vector<HexTile>>    Tiles;

	//���� ��
    float           Alpha = 1.0f;

	//����ź������� ���������� ���
    UINT findH(POINT from,POINT to);


public:
    
    HRESULT init();
    void release();
    void update();
    void render();
	//Ÿ�ϸʿ� ���� �̹��� �߰�
    void AddImage(_tstring file, UINT MaxFrameX, UINT MaxFrameY,
        string vs = "VS", string ps = "PS");
	//Ÿ����ġ �ٽ����
    void InitPosition();
	//Ÿ�� ���� ����
	void ClearTile();
	//Ÿ�� �� �Ҵ�
	void ResizeTile();

    //��ã��
    bool PathFinding(POINT from,POINT to,
        vector<HexTile*>& way);
    
	//������ �� �Լ�
    MAKEREFGET(Vector2 , TileSize)
    MAKEREFGET(POINT  ,TileMax)
    MAKEREFGET(Vector2  ,LB)
    MAKEREFGET(vector<Image*>  ,vecTile)
    MAKEREFGET(vector<vector<HexTile>>  ,  Tiles)
    MAKEREFGET(float  ,Alpha)
};



