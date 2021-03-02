#pragma once
#define EVEN 1
#define ODD -1


//큐브 좌표
struct Hex
{
	const int q, r, s;
	Hex(int q_, int r_, int s_) :q(q_), r(r_), s(s_)
	{
		assert(q + r + s == 0,"q+r+s는 0이상이어야합니다.");
	}

};

//멤버 타입이 다른 헥사
struct FractionalHex
{
	const double q, r, s;
	FractionalHex(double q_, double r_, double s_)
		: q(q_), r(r_), s(s_) {}
};

struct OffsetCoord
{
	const int col, row;
	OffsetCoord(int col_, int row_) : col(col_), row(row_) {}
};



//연산자 정의 ==
bool operator ==(Hex a, Hex b)
{
	return a.q == b.q && a.r == b.r && a.s == b.s;
}

//연산자 정의 !=
bool operator !=(Hex a, Hex b)
{
	return !(a == b);
}

struct Point
{
	const double x, y;
	Point(double x_, double y_) : x(x_), y(y_) {}
};



//2*2 순방향 행렬, 2*2 역행렬 및 시작각도를 정하기 위해 만든 도우미클래스
struct Orientation {
	const double f0, f1, f2, f3;
	const double b0, b1, b2, b3;
	const double start_angle; // in multiples of 60°
	Orientation(double f0_, double f1_, double f2_, double f3_, double b0_, double b1_, double b2_, double b3_, double start_angle_)
		: f0(f0_), f1(f1_), f2(f2_), f3(f3_), b0(b0_), b1(b1_), b2(b2_), b3(b3_), start_angle(start_angle_) {}
};

struct Layout {
	const Orientation orientation;
	const Point size;
	const Point origin;
	Layout(Orientation orientation_, Point size_, Point origin_)
		: orientation(orientation_), size(size_), origin(origin_) {}
};


namespace std {
	template <> struct hash<Hex> {
		size_t operator()(const Hex& h) const
		{
			hash<int> int_hash;
			size_t hq = int_hash(h.q);
			size_t hr = int_hash(h.r);
			return hq ^ (hr + 0x9e3779b9 + (hq << 6) + (hq >> 2));
		}
	};
}


//
////직사각형 뾰족한형태의 맵
//template<class T> 
//class RectangularPointyTopMap
//{
//
//private:
//
//	vector<vector<T>> map;
//	int width;
//	int height;
//
//
//public:
//	map(int _width, int _height) : width(_width),height(_height)
//	{
//		for (int r = 0; r < height; r++)
//		{
//			map.emplace_back(width);
//		}
//	}
//
//	inline T& at(int q, int r) {
//		return map[r][q + (r >> 1)];
//	}
//};


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
	Point hex_corner_offset(Layout layout, int corner);

	//벡터포인트 반환 코너구해주는 함수
	vector<Point> polygon_corners(Layout layout, Hex h);

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

	//분수핵사를 반올림해주는 함수
	Hex hex_round(FractionalHex h);

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


	//RectangularPointyTopMap<Hex> map;
	
	//헥사를 픽셀화면으로
	Point hex_to_pixel(Layout layout, Hex h);

	//픽셀을 핵사화면으로
	FractionalHex pixel_to_hex(Layout layout, Point p);


	//핵사 라인 그리기
	vector<Hex> hex_linedraw(Hex a, Hex b);

	//핵사 맵 만들기 육각형
	void MapCreaterHexagons();

	//핵사 맵 만들기 직사각형
	void MapCreaterRectangles();

	//오프셋 좌표로 변환
	OffsetCoord qoffset_from_cube(int offset, Hex h);

	//큐브좌표로 변환
	Hex qoffset_to_cube(int offset, OffsetCoord h);


};



