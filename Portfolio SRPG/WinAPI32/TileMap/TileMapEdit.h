#pragma once
#define NULLNUMBER 9898
#define EVEN 1
#define ODD -1

#pragma region Hex Coord


//cube ��ǥ
struct Hex
{
	// x , z ,y 
	int q, r, s;
	Hex(int q_, int r_, int s_) :q(q_), r(r_), s(s_)
	{
		assert(q + r + s == 0);
	}
	Hex(int q_, int r_) :q(q_), r(r_)
	{
		s = -q - r;
		assert(q + r + s == 0);
	}
	Hex()
	{
	}

	friend std::hash<Hex>;

	friend bool operator==(const Hex& a, const Hex& b);
	friend bool operator!=(const Hex& p1, const Hex& p2);

};

//��� Ÿ���� cube ��ǥ
struct FractionalHex
{
	 double q, r, s;
	FractionalHex(double q_, double r_, double s_)
		: q(q_), r(r_), s(s_) {}
	FractionalHex() {}
};

//ofset ��ǥ
struct OffsetCoord
{
	int col, row;
	OffsetCoord(int col_, int row_) : col(col_), row(row_) {}
	OffsetCoord() { col = NULLNUMBER; row = NULLNUMBER; }
	friend std::hash<OffsetCoord>;
	friend bool operator==(const OffsetCoord& p1, const OffsetCoord& p2);

};


#pragma endregion


#pragma region �ؽ��Լ�

namespace std {
	template <>
	class hash<OffsetCoord> {
	public:
		size_t operator()(const OffsetCoord &x2) const {
			using std::hash;

			return		hash<int>()(x2.col) ^ hash<float>()(x2.row) << 1;
		}
	};
};

namespace std {
	template <> struct hash<Hex> {
		size_t operator()(const Hex& h) const {
			hash<int> int_hash;
			size_t hq = int_hash(h.q);
			size_t hr = int_hash(h.r);
			return hq ^ (hr + 0x9e3779b9 + (hq << 6) + (hq >> 2));
		}
	};
}

#pragma endregion



//2*2 ������ ���, 2*2 ����� �� ���۰����� ���ϱ� ���� ���� �����Ŭ����
struct Orientation {
	 double f0, f1, f2, f3;
	 double b0, b1, b2, b3;
	 double start_angle; // in multiples of 60��
	Orientation(double f0_, double f1_, double f2_, double f3_, double b0_, double b1_, double b2_, double b3_, double start_angle_)
		: f0(f0_), f1(f1_), f2(f2_), f3(f3_), b0(b0_), b1(b1_), b2(b2_), b3(b3_), start_angle(start_angle_) {}
	Orientation() {}
};

struct Layout {
	Orientation orientation;
	Vector2 size;
	Vector2 origin;
	Layout(Orientation orientation_, Vector2 size_, Vector2 origin_)
		: orientation(orientation_), size(size_), origin(origin_) {}
	Layout() {}
};



enum class TILESTATE
{
	TILE_NONE,
	TILE_WALL,
};

struct HexTile
{

	Hex cubeCoord;				//ť����ǥ
	OffsetCoord offCoord;		//������ ��ǥ
	Vector2 Pos;				//Ÿ���� ��ǥ
	UINT vecIdx;				//�̹��� ��ȣ
	POINT imgIdx;				//�̹��� �ε���
	TILESTATE tileState;		//Ÿ���� ����
	bool check = false;			//Ÿ���� ���õǾ� �ִ� ����

	//Ÿ�� ������Ʈ(����)
	HexTile() :vecIdx(0)
	{
		imgIdx.x = 0;
		imgIdx.y = 0;
		tileState = TILESTATE::TILE_NONE;
	}

	void CheckSwiching()
	{
		check = !check;
	}
#pragma region A* �˰��� �ʿ��� ������
	//UINT F;					 //������ + ����������
	//UINT G;					 //����������
	//UINT H;					 //������
	//Tile* P;					//�������� ���ŵǾ��°�?
#pragma endregion

};


//���簢�� ������������ ��
class RectangularPointyTopMap
{

private:

	//�������� �̹����� ����Ҽ� �����Ƿ�
	vector<Image*>  tileImg;

	unordered_map<OffsetCoord, Hex> hTiles;


public:	

	int width;									//���� ũ��
	int height;									//���� ũ��
	float Alpha = 0.8f;							//Ÿ������
	
	Vector2 TileSize;							//Ÿ�� ������

	//�⺻ ������
	RectangularPointyTopMap(){};

	//Ÿ���� ������ ��ġ
	Vector2 LB;
	
	vector<vector<HexTile>> Tiles;

	//�̹��� ��������	
	void AddImage(_tstring file, UINT MaxFrameX, UINT MaxFrameY, string vs = "VS", string ps = "PS");

	//�ʱ�ȭ
	void Init();

	void Update();

	//Ÿ�� �Ҵ�
	void ResizeTile();
	
	//Ÿ�ϵ� ��ġ ���
	void InitPosition();

	//���� �����
	void ClearTile();

	//Ÿ�� �׸���
	void Reneder();

};



class TileMapEdit
{

private:
	
	//6���� ����
	const vector<Hex> hex_directions = {
	Hex(1, 0, -1), Hex(1, -1, 0), Hex(0, -1, 1),
	Hex(-1, 0, 1), Hex(-1, 1, 0), Hex(0, 1, -1)
	};

	//������ ����
	const Orientation layout_pointy = Orientation(sqrt(3.0), sqrt(3.0) / 2.0, 0.0, 3.0 / 2.0, sqrt(3.0) / 3.0, -1.0 / 3.0, 0.0, 2.0 / 3.0, 0.5);
	//������ ����
	const Orientation layout_flat = Orientation(3.0 / 2.0, 0.0, sqrt(3.0) / 2.0, sqrt(3.0), 2.0 / 3.0, 0.0, -1.0 / 3.0, sqrt(3.0) / 3.0, 0.0);
	
	//�ٻ� �𼭸� ���ϱ�
	Vector2 hex_corner_offset(Layout layout, int corner);

	//��������Ʈ ��ȯ �ڳʱ����ִ� �Լ�
	vector<Vector2> polygon_corners(Layout layout, Hex h);

	//��� ���ϱ�
	Hex hex_add(Hex a, Hex b);
	//��� ����
	Hex hex_subtract(Hex a, Hex b);
	//��� ���ϱ�
	Hex hex_multiply(Hex a, int k);

	//��� �Ÿ����ϱ�
	int hex_length(Hex hex);
	//��� ���� �Ÿ� ���ϱ�
	int hex_distance(Hex a, Hex b);

	//������ ������� �Լ�
	Hex hex_direction(int direction /* 0 to 5 */);

	//����� �̿����ϱ�
	Hex hex_neighbor(Hex hex, int direction);



	//����ȸ��
	Hex hex_rotate_left(Hex a);
	//������ȸ��
	Hex hex_rotate_right(Hex a);

	//�������� �Լ�
	float lerp(double a, double b, double t);

	//�ε��Ҽ� �ٻ縦 ���������ϰ� ��ȯ�ϴ� �Լ�
	FractionalHex hex_lerp(Hex a, Hex b, double t);

	////�ٻ� ���� �׸��� (������ ������ �����ִ� ��)
	//FractionalHex hex_lerp(FractionalHex a, FractionalHex b, double t);


public:

	TileMapEdit() {};

	//�����簢�� ĸ��ȭ�ϴ� ����
	RectangularPointyTopMap map;


	//���̾ƿ�
	Layout layout;

	//�������� �ȼ���
	Vector2 oddr_offset_to_pixel(OffsetCoord a,float size)
	{
		float x = size * sqrt(3) * (a.col + 0.5 * (a.row & 1));
		float y = size * 3 / 2 * a.row;
		return Vector2(x, y);
	}


	//��縦 �ȼ�ȭ������
	Vector2 hex_to_pixel(Layout layout, Hex h);

	//�ȼ��� �ٻ�ȭ������
	FractionalHex pixel_to_hex(Layout layout, Vector2 p);

	//�м��ٻ縦 �ݿø����ִ� �Լ�
	Hex hex_round(FractionalHex h);

	//�ٻ� ���� �׸���
	vector<Hex> hex_linedraw(Hex a, Hex b);

	//������ q��ǥ�� ��ȯ
	OffsetCoord cube_to_roffset(int offset, Hex h);

	//ť����ǥ�� ��ȯ
	Hex roffset_to_cube(int offset, OffsetCoord h);

	//������ ��������ǥ ��ȯ
	OffsetCoord cube_to_oddr(Hex a);
	
	

	//�ʱ�ȭ
	HRESULT Init();
	
	//���� �� ȣ��
	void Release();

	//������Ʈ
	void Update();

	//����
	void Render();
	

};



#pragma region ����Ŭ����/*
template<class T> class HexMap {

	vector<vector<T>> map;


public:


	void Map(int width_, int height_)
	{
		map.resize(height_);
		for (int r = 0; r < height_; r++) {
			map.emplace_back(width_);
		}
	}

	inline T& at(int q, int r) {
		return map[r][q + (r >> 1)];
	}

};*/
#pragma endregion
