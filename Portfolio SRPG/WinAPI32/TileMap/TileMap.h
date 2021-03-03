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
struct Tile
{

#pragma region A* �˰��� �ʿ��� ������
	UINT F;					 //������ + ����������
	UINT G;					 //����������
	UINT H;					 //������
	Tile* P;				 //�������� ���ŵǾ��°�?
#pragma endregion

    
    POINT TileIdx;			 //���� Ÿ�� �ε���

    bool  isFind;			 //Ž���� �ߴ� Ÿ���ΰ�?

    UINT vecIdx;			 //���° �̹���?
    POINT ImgIdx;			 //���° ����?

    int state;				 //Ÿ�� ����
    Vector2 Pos;			 //Ÿ���� ��ġ LB���� ������

	Tile():state(TILE_NONE),vecIdx(0)
    {
        ImgIdx.x = 0;
        ImgIdx.y = 0;
		
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
	vector<vector<Tile>>    Tiles;

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
    bool PathFinding(POINT from,POINT to, vector<Tile*>& way);
    
	//������ �� �Լ�
    MAKEREFGET(Vector2 , TileSize)
    MAKEREFGET(POINT  ,TileMax)
    MAKEREFGET(Vector2  ,LB)
    MAKEREFGET(vector<Image*>  ,vecTile)
    MAKEREFGET(vector<vector<Tile>>  ,  Tiles)
    MAKEREFGET(float  ,Alpha)
};



