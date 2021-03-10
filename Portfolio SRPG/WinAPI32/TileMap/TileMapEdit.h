#pragma once
#define NULLNUMBER 9898
#define EVEN 1
#define ODD -1




enum class TILESTATE
{
	TILE_NONE,
	TILE_WALL,
};

#pragma region Hex Coord


//cube ��ǥ
class Hex
{
	friend std::hash<Hex>;
	friend bool operator==(const Hex& a, const Hex& b);
	friend bool operator!=(const Hex& p1, const Hex& p2);

public:

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

	Hex(){}

	Vector2 Pos;
	UINT vecIdx = 0;										//�̹��� ��ȣ
	POINT imgIdx = POINT{ 0,0 };							//�̹��� �ε���
	TILESTATE tileState = TILESTATE::TILE_NONE;				//Ÿ���� ����
	bool check = false;										//Ÿ���� ���õǾ� �ִ� ����
	void CheckSwiching(){check = !check;}					//���½���ġ

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
	OffsetCoord(int col_, int row_) : col(row_), row(col_) {}
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

//���簢���� ������ �� Ŭ���� 
class RectangularPointyTopMap 
{
private:

public:

	int width;									//���� �ʺ�
	int height;									//���� ����
	float Alpha = 0.5f;							//Ÿ������
	Vector2 LB;									//Ÿ���� ������ ��ġ
	Vector2 TileSize;							//Ÿ�� ������


	vector<Image*> tileImg;						//Ÿ�� �̹�����
	vector<vector<Hex>> tiles;					//Ÿ�ϵ�
	//unordered_set<Hex> map;
	unordered_map<OffsetCoord, Hex> map;
	


	//������
	RectangularPointyTopMap(){}

	void Init();

	//Ÿ�� �Ҵ�
	void ResizeTile();

	//Ÿ���� ��ġ
	void TilePositoinInit();

	//���� �Ҹ��� �� �Ҹ��� �����ִ� �Լ�
	void ClearTile();

	//Ÿ���� �׸��� �Լ�
	void Reneder();

	
	//void CreateMap()
	//{
	//	for (int r = -10; r < height; r++) {
	//		int r_offset = floor(r / 2); // or r>>1
	//		for (int q = -r_offset; q < width - r_offset; q++) {
	//			map.insert(Hex(q, r, -q - r));
	//		}
	//	}
	//}


	/*inline Hex& at(int q, int r) {
		return tiles[r][q + (r >> 1)];
	}*/
						
	inline Hex& at(int q, int r) {
		return tiles[r][q + (r >> 1)];
	}

	//�̹��� ��������	
	void AddImage(_tstring file, UINT MaxFrameX, UINT MaxFrameY, string vs = "VS", string ps = "PS");

};




class TileMapEdit
{

private:
	
	//vector<HexTile*> selectTiles;


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
	//unordered_set<Hex> hexs;			����

	

	//���õ� Ÿ�ϵ�
	unordered_map<OffsetCoord, Hex*> selectTiles;


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
	
	
	//��� ���� �Ÿ� ���ϱ�
	int hex_distance(Hex a, Hex b);

	//Ÿ�� �浹
	bool PtInHexTile(Vector2 Pt, OUT OffsetCoord& coord);


	//�ʱ�ȭ
	HRESULT Init();
	
	//���� �� ȣ��
	void Release();

	//������Ʈ
	void Update();

	//����
	void Render();
	

};

