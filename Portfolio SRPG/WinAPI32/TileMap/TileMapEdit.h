#pragma once
#define EVEN 1
#define ODD -1


//ť�� ��ǥ
struct Hex
{
	const int q, r, s;
	Hex(int q_, int r_, int s_) :q(q_), r(r_), s(s_)
	{
		assert(q + r + s == 0,"q+r+s�� 0�̻��̾���մϴ�.");
	}

};

//��� Ÿ���� �ٸ� ���
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



//������ ���� ==
bool operator ==(Hex a, Hex b)
{
	return a.q == b.q && a.r == b.r && a.s == b.s;
}

//������ ���� !=
bool operator !=(Hex a, Hex b)
{
	return !(a == b);
}

struct Point
{
	const double x, y;
	Point(double x_, double y_) : x(x_), y(y_) {}
};



//2*2 ������ ���, 2*2 ����� �� ���۰����� ���ϱ� ���� ���� �����Ŭ����
struct Orientation {
	const double f0, f1, f2, f3;
	const double b0, b1, b2, b3;
	const double start_angle; // in multiples of 60��
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
////���簢�� ������������ ��
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
	Point hex_corner_offset(Layout layout, int corner);

	//��������Ʈ ��ȯ �ڳʱ����ִ� �Լ�
	vector<Point> polygon_corners(Layout layout, Hex h);

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

	//�м��ٻ縦 �ݿø����ִ� �Լ�
	Hex hex_round(FractionalHex h);

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


	//RectangularPointyTopMap<Hex> map;
	
	//��縦 �ȼ�ȭ������
	Point hex_to_pixel(Layout layout, Hex h);

	//�ȼ��� �ٻ�ȭ������
	FractionalHex pixel_to_hex(Layout layout, Point p);


	//�ٻ� ���� �׸���
	vector<Hex> hex_linedraw(Hex a, Hex b);

	//�ٻ� �� ����� ������
	void MapCreaterHexagons();

	//�ٻ� �� ����� ���簢��
	void MapCreaterRectangles();

	//������ ��ǥ�� ��ȯ
	OffsetCoord qoffset_from_cube(int offset, Hex h);

	//ť����ǥ�� ��ȯ
	Hex qoffset_to_cube(int offset, OffsetCoord h);


};



