#pragma once
#define NULLNUMBER 9898
#define EVEN 1
#define ODD -1

#pragma region Hex Coord


//cube 좌표
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

//멤버 타입이 cube 좌표
struct FractionalHex
{
	 double q, r, s;
	FractionalHex(double q_, double r_, double s_)
		: q(q_), r(r_), s(s_) {}
	FractionalHex() {}
};

//ofset 좌표
struct OffsetCoord
{
	int col, row;
	OffsetCoord(int col_, int row_) : col(col_), row(row_) {}
	OffsetCoord() { col = NULLNUMBER; row = NULLNUMBER; }
	friend std::hash<OffsetCoord>;
	friend bool operator==(const OffsetCoord& p1, const OffsetCoord& p2);

};


#pragma endregion


#pragma region 해시함수

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



//2*2 순방향 행렬, 2*2 역행렬 및 시작각도를 정하기 위해 만든 도우미클래스
struct Orientation {
	 double f0, f1, f2, f3;
	 double b0, b1, b2, b3;
	 double start_angle; // in multiples of 60°
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

	Hex cubeCoord;				//큐브좌표
	OffsetCoord offCoord;		//오프셋 좌표
	Vector2 Pos;				//타일의 좌표
	UINT vecIdx;				//이미지 번호
	POINT imgIdx;				//이미지 인덱스
	TILESTATE tileState;		//타일의 상태
	bool check = false;			//타일이 선택되어 있는 상태

	//타일 스테이트(상태)
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
#pragma region A* 알고리즘에 필요한 변수들
	//UINT F;					 //예상비용 + 현재까지비용
	//UINT G;					 //현재까지비용
	//UINT H;					 //예상비용
	//Tile* P;					//누굴통해 갱신되었는가?
#pragma endregion

};


//직사각형 뾰족한형태의 맵
class RectangularPointyTopMap
{

private:

	//여러장의 이미지를 사용할수 있으므로
	vector<Image*>  tileImg;

	unordered_map<OffsetCoord, Hex> hTiles;


public:	

	int width;									//가로 크기
	int height;									//세로 크기
	float Alpha = 0.8f;							//타일투명도
	
	Vector2 TileSize;							//타일 사이즈

	//기본 생성자
	RectangularPointyTopMap(){};

	//타일의 시작점 위치
	Vector2 LB;
	
	vector<vector<HexTile>> Tiles;

	//이미지 가져오기	
	void AddImage(_tstring file, UINT MaxFrameX, UINT MaxFrameY, string vs = "VS", string ps = "PS");

	//초기화
	void Init();

	void Update();

	//타일 할당
	void ResizeTile();
	
	//타일들 위치 잡기
	void InitPosition();

	//벡터 지우기
	void ClearTile();

	//타일 그리기
	void Reneder();

};



class TileMapEdit
{

private:
	
	//6가지 방향
	const vector<Hex> hex_directions = {
	Hex(1, 0, -1), Hex(1, -1, 0), Hex(0, -1, 1),
	Hex(-1, 0, 1), Hex(-1, 1, 0), Hex(0, 1, -1)
	};

	//뾰족한 방향
	const Orientation layout_pointy = Orientation(sqrt(3.0), sqrt(3.0) / 2.0, 0.0, 3.0 / 2.0, sqrt(3.0) / 3.0, -1.0 / 3.0, 0.0, 2.0 / 3.0, 0.5);
	//평평한 방향
	const Orientation layout_flat = Orientation(3.0 / 2.0, 0.0, sqrt(3.0) / 2.0, sqrt(3.0), 2.0 / 3.0, 0.0, -1.0 / 3.0, sqrt(3.0) / 3.0, 0.0);
	
	//핵사 모서리 구하기
	Vector2 hex_corner_offset(Layout layout, int corner);

	//벡터포인트 반환 코너구해주는 함수
	vector<Vector2> polygon_corners(Layout layout, Hex h);

	//헥사 더하기
	Hex hex_add(Hex a, Hex b);
	//헥사 빼기
	Hex hex_subtract(Hex a, Hex b);
	//헥사 곱하기
	Hex hex_multiply(Hex a, int k);

	//헥사 거리구하기
	int hex_length(Hex hex);
	//헥사 사이 거리 구하기
	int hex_distance(Hex a, Hex b);

	//방향을 갖고오는 함수
	Hex hex_direction(int direction /* 0 to 5 */);

	//헥사의 이웃구하기
	Hex hex_neighbor(Hex hex, int direction);



	//왼쪽회전
	Hex hex_rotate_left(Hex a);
	//오른쪽회전
	Hex hex_rotate_right(Hex a);

	//선형보간 함수
	float lerp(double a, double b, double t);

	//부동소수 핵사를 선형보간하고 반환하는 함수
	FractionalHex hex_lerp(Hex a, Hex b, double t);

	////핵사 라인 그리기 (러프의 오류를 막아주는 것)
	//FractionalHex hex_lerp(FractionalHex a, FractionalHex b, double t);


public:

	TileMapEdit() {};

	//직각사각형 캡슐화하는 변수
	RectangularPointyTopMap map;


	//레이아웃
	Layout layout;

	//오프셋을 픽셀로
	Vector2 oddr_offset_to_pixel(OffsetCoord a,float size)
	{
		float x = size * sqrt(3) * (a.col + 0.5 * (a.row & 1));
		float y = size * 3 / 2 * a.row;
		return Vector2(x, y);
	}


	//헥사를 픽셀화면으로
	Vector2 hex_to_pixel(Layout layout, Hex h);

	//픽셀을 핵사화면으로
	FractionalHex pixel_to_hex(Layout layout, Vector2 p);

	//분수핵사를 반올림해주는 함수
	Hex hex_round(FractionalHex h);

	//핵사 라인 그리기
	vector<Hex> hex_linedraw(Hex a, Hex b);

	//오프셋 q좌표로 변환
	OffsetCoord cube_to_roffset(int offset, Hex h);

	//큐브좌표로 변환
	Hex roffset_to_cube(int offset, OffsetCoord h);

	//검증된 오프셋좌표 변환
	OffsetCoord cube_to_oddr(Hex a);
	
	

	//초기화
	HRESULT Init();
	
	//삭제 시 호출
	void Release();

	//업데이트
	void Update();

	//렌더
	void Render();
	

};



#pragma region 보류클래스/*
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
